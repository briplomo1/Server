//
// Created by b1n4r33 on 3/10/25.
//

#pragma once

#define BUFFER_SIZE 4096
#include <functional>
#include <iostream>
#include <queue>
#include <waiter/types.h>
#include <waiter/endpoint/endpoint.h>
#include <waiter/io/IOContext.h>
#include <waiter/io/WinSockManager.h>


#ifdef _WIN32
    #include <ws2tcpip.h>
    #include <winsock2.h>
    #define INVALID_SOCK INVALID_SOCKET
    #define SOCK_ERROR SOCKET_ERROR
#elifdef LINUX

    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <errno.h>

    #define INVALID_SOCK (-1)
    #define SOCK_ERROR (-1)
#endif


/**
 * What properties and methods are common to all socket and protocol types will be implemented in this SScoetk class.
 * Specific protocols and socket types will extend virtual methods
 */
namespace Waiter {

    typedef addrinfo AddressInfo;
#ifdef _WIN32
    typedef SOCKET SocketDescriptor;
#else
    typedef int SocketDescriptor;
#endif


    template<ADDR_FAM AddressFamily, SOCK_TYPE SocketType, PROTOCOL Protocol, bool IsServer, bool IsDualStack=true>
    class BaseSocket {

        using EndpointType = Endpoint<AddressFamily, SocketType, Protocol, IsServer, IsDualStack>;

        EndpointType local_endpoint_;
        EndpointType remote_endpoint_;

        SocketDescriptor fd;

        std::queue<std::string> send_queue_;
        std::queue<std::string> recv_queue_;

        bool wants_read_;
        bool wants_write_;

        void setNoDelay() {
            if (SocketType == SOCK_STREAM) {
                int one = 1;
                int res;

                #if defined(_WIN32) || defined(_WIN64)
                // Windows expects const char* for option value
                res = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<const char*>(&one), sizeof(one));
                if (res == SOCKET_ERROR) {
                    std::cerr << "setsockopt failed: " << WSAGetLastError() << std::endl;
                    std::exit(1);
                }
                #else
                // Linux/Unix
                res = setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
                if (res == -1) {
                    perror("setsockopt failed");
                    std::exit(1);
                }
                #endif

            }
        }

        void setNonBlockingAndCloExec()  {
            #if defined(__linux__) || defined(__APPLE__)
            // Non-blocking
            int flags = fcntl(fd, F_GETFL, 0);
            fcntl(fd, F_SETFL, flags | O_NONBLOCK);

            // Close-on-exec
            flags = fcntl(fd_set, F_GETFD, 0);
            fcntl(fd_set, F_SETFD, flags | FD_CLOEXE);

            #elif defined(_WIN32) || defined(_WIN64)

            u_long mode = 1; // 1 = non-blocking
            ioctlsocket(fd, FIONBIO, &mode);

            // Disable handle inheritance
            SetHandleInformation(reinterpret_cast<HANDLE>(fd), HANDLE_FLAG_INHERIT, 0);
            #endif
        }

        void configureIP() {
            if constexpr (AddressFamily == IPV6) {
                constexpr int opt = IsDualStack ? 0: 1;
                    setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, reinterpret_cast<const char*>(&opt), sizeof(opt));
            }
        }

        void bindIfServer() {
            if constexpr (IsServer) {
                // TODO: bind address from endpoint
            }
        }

        bool wouldBlock() {
        #ifdef _WIN32
            int err = WSAGetLastError();
            return err == WSAEWOULDBLOCK;
        #elifdef LINUX
            return errno == EAGAIN || errno == EWOULDBLOCK;
        #endif
        }

    protected:

        BaseSocket() : local_endpoint_(EndpointType{}), remote_endpoint_(), fd(INVALID_SOCK), wants_read_(false), wants_write_(false) {
            // Initialize WinSock if WINDOWS
            WinSockManager::initialize();

            // Initialize socket descriptor
            fd = socket(AddressFamily, SocketType, Protocol);

            // Check for socket error
            if (fd == INVALID_SOCK) {
                std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
                std::exit(1);
            }

            setNoDelay();
            setNonBlockingAndCloExec();
            configureIP();
            bindIfServer();

            std::cout << "Socket created: " << fd << std::endl;
            //IOContext<IOPoller>::instance().register_socket(*this);
        }

        /**
         *  When a {@link BaseSocket} instance wants to write data, which is expressed
         *  through {@link wants_write_}, the polling code, which may be a {@link IOPoller} will call
         *  this function to handle writing out data that is queued in the instance's {@link send_queue_}.
         */
        void on_writeable() {
            while (!send_queue_.empty()) {
                std::string &msg = send_queue_.front();
                const ssize_t res =
                #ifdef _WIN32
                    send(fd, msg.data(), static_cast<int>(msg.size()), 0);
                #elifdef LINUX
                    ::send(fd, msg.data(), msg.size(), 0);
                #endif

                if (res > 0) {
                    msg.erase(0, res);
                    if (msg.empty()) {
                        send_queue_.pop();
                    }
                } else {
                    if (wouldBlock()) {
                        wants_write_ = true;
                        break;
                    }
                    wants_write_ = false;
                    // TODO: handle error
                    return;
                }
                handle_write();
            }

            if (send_queue_.empty()) {
                wants_write_ = false;
            }

            // TODO: Add optional user supplied post flush hook

        }

        /**
         *  When a {@link BaseSocket} has expressed its interest in reading data. User code
         *  or a {@link IOPoller} will call this function to handle reading/receiving data and
         *  queueing the read data into this instance's {@link recv_queue_} to be handled later by
         *  user {@link handle_read} implementation.
         */
        void on_readable() {
            char buffer[BUFFER_SIZE];
            const ssize_t res =
            #ifdef _WIN32
                recv(fd, buffer, BUFFER_SIZE, 0);
            #elifdef LINUX
                ::recv(fd, buffer, BUFFER_SIZE, 0);
            #endif
            if (res > 0) {
                //TODO: Efficient?
                recv_queue_.emplace(std::string(buffer, res));
                handle_read();
            } else if (res == 0)
            {
                handle_error();
            } else {
                if (wouldBlock()) return;
                handle_error();
            }

        }

        /**
         *  To be called by managing code if error occurs on socket operation. Will call hook
         *  {@link handle_error} implementation.
         */
        void on_error() {
            handle_error();
        }

        /**
         *  Shutdown socket and release resources with call to close
         */
        void close_socket() {
        #ifdef _WIN32
            shutdown(fd, SD_BOTH);
            closesocket(fd);
            std::cout << "Socket destroyed: " << fd << std::endl;
        #else
            shutdown(fd, SHUT_RDWR);
            close(fd);
        #endif
            //IOContext<>::instance().deregister_socket(*this);
        }

    public:

        // No copies of BaseSocket
        BaseSocket(const BaseSocket&) = delete;

        BaseSocket& operator=(const BaseSocket&) = delete;

        // No move assignment
        BaseSocket& operator=(BaseSocket&&) noexcept = delete;

        /**
         * Move constructor moves most resources into current {@link BaseSocket} instance from other
         * except integral types. Will invalidate other's socket file descriptor.
         * @param other The {@link BaseSocket} that is to be moved.
         */
        BaseSocket(BaseSocket&& other) noexcept
        : local_endpoint_(std::move(other.local_endpoint_)),
        remote_endpoint_(std::move(other.remote_endpoint_)),
        fd(other.fd),
        send_queue_(std::move(other.send_queue_)),
        recv_queue_(std::move(other.recv_queue_)),
        wants_read_(other.wants_read_),
        wants_write_(other.wants_write_) {
            other.fd = INVALID_SOCK;
        }

        void connect_remote(const EndpointType& endpoint)  requires (SocketType == DATAGRAM && !IsServer) {

            if (connect(fd, remote_endpoint_.data(), remote_endpoint_.size()) == SOCKET_ERROR) {
                perror("connect failed");
                throw std::runtime_error("Socket connect failed");
            }
            remote_endpoint_ = endpoint;
        }

        void listen_on(int backlog = 100) requires (SocketType == STREAM && IsServer) {
            if (listen(fd, backlog) == SOCK_ERROR) {
                perror("listen failed");
                throw std::runtime_error("Socket listen failed");
            }
        }

        // TODO: can datagram accept connection?
        BaseSocket* accept_connection() requires (SocketType == STREAM && IsServer) {
            SocketDescriptor client_fd;
            AddressStorage client_addr{};
            socklen_t cient_socklen = sizeof(client_addr);
        }

        /**
         * Exposes internal state of {@link BaseSocket} representing its interest to read
         * incoming IO data. This method is called by a {@link IOPoller} in order to gauge
         * which polling operations to conduct in the socket.
         *
         * @return Returns boolean representing if socket desires to read data
         */
        [[nodiscard]] bool wants_read() const { return wants_read_; }

        /**
         * Exposes internal state of {@link BaseSocket} representing its interest to write
         * outgoing IO data. This method is called by a {@link IOPoller} in order to gauge
         * which polling operations to conduct in the socket.
         *
         * @return Returns boolean representing if socket desires to write data
         */
        [[nodiscard]] bool wants_write() const { return wants_write_; }

        /**
         * Sets the internal state of {@link BaseSocket} representing its desire to read
         * data through an IO operation. Is to be set any time a read/recv operation
         * needs to take place, and to false when no read operation needs to take place.
         */
        void set_read(const bool b) {  wants_read_ = b; }

        /**
         * Sets the internal state of {@link BaseSocket} representing its desire to write
         * data through an IO operation. Is to be set to true any time a write/send operation
         * needs to take place, and to false when no write operation needs to take place.
         */
        void set_write(const bool b) {  wants_write_ = b; }

        /**
         *  Hook to be called when messages are available in {@link recv_queue_}.
         *  Implementation will be protocol/application specific to handle received messages.
         */
        void virtual handle_read() = 0; // Handle messages in recv q

        /**
         *  Hook to be called when a IO error occurs in a {@link BaseSocket} operation.
         *  Will handle protocol/application specific cleanup and error handling.
         */
        void virtual handle_error() = 0; // Protocol specific cleanup triggered on error

        /**
         * Hook to be called after data is sent successfully.
         * May be optional or mandatory depending on protocol/application.
         */
        void virtual handle_write() = 0; // Hook after send is done

        /**
         * Gets system native socket/file descriptor. Should rarely be used unless setting additional
         * flags/options on socket.
         * @return The native system's socket descriptor for raw usage.
         */
        [[nodiscard]] SocketDescriptor get_socket() const { return fd; }

        /**
         *  Virtual destructor does socket resource cleanup. Implementation will do
         *  protocol/application specific cleanup and resource management.
         */
        virtual ~BaseSocket() {
            close_socket();
        }

    };

}



