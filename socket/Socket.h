//
// Created by b1n4r33 on 3/10/25.
//

#pragma once

#define BUFFER_SIZE 1024
#define DEFAULT_ADDRESS "127.0.0.1"
#define DEFAULT_PORT "8000"

#include "SocketArgs.h"
#include <cstdint>
#include <memory>
#include <new>
#include <vector>


#ifdef _WIN32
    #include <ws2tcpip.h>

    #define INVALID_SOCK INVALID_SOCKET
    #define SOCK_ERROR SOCKET_ERROR
#else

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
namespace Waiter::Networking {

    typedef addrinfo AddressInfo;
    #ifdef _WIN32
    typedef SOCKET SocketDescriptor;
    #else
    typedef int SocketDescriptor;
    #endif

    class Socket {
        // User defined socket connection properties
        SOCK_FAM addressFamily;
        SOCK_TYPE socketType;
        SOCK_PROTOCOL socketProtocol = DEFAULT;
        SOCK_ADDRESS address;
        SOCK_PORT port;

        // Socket file descriptor
        SocketDescriptor socketDescriptor;

        /**
         * Vector of addresses found for host by call to {@link getaddrinfo}.
         * Will contain IPV4 and IPV6 addresses if dull stack sockets is enabled.
         * May contain more addresses if host has more than one network interface.
         */
        std::vector<AddressInfo> addresses;

    public:

        /**
         * Socket should not be copied
         */
        Socket(const Socket&) = delete;

        /**
         * Socket should not be copied
         */
        Socket& operator=(const Socket&) = delete;

        /**
         * Move constructor
         *
         */
        Socket(Socket&&) noexcept;

        /**
         * Move assignment operator
         * @return
         */
        Socket& operator=(Socket&&) noexcept;

        /**
         * Non exception throwing constructor. Will terminate process on failure.
         * @param addressFamily 
         * @param socketType 
         * @param socketProtocol 
         */
        Socket(SOCK_FAM addressFamily, SOCK_TYPE socketType, SOCK_PROTOCOL socketProtocol, std::nothrow_t);

        /**
         * Clean up resources. Will do common resource closing such as closing socket.
         * Protocol and socket type will define further resource cleanup.
         */
        virtual ~Socket();

        /**
         * Specific to server sockets
         * Given a file descriptor, associate or bind the socket file descriptor with a port and address.
         * Is only
         * @param address The address to bind to
         * @param port The port number to bind too
         */
        void bindSocket(const std::string &address, const std::string &port);

        /**
         * Specific to client socket
         * @param address 
         * @param port 
         */
        void connectSocket(const std::string &address, const std::string &port);

        /**
         * Specific to server socket.
         * Accepts an incoming connection.
         */
        void acceptConnection();

        /**
         *
         * @param client_socket 
         * @return 
         */
        [[nodiscard]] std::vector<char> receive(int client_socket) const ;

        /**
         * 
         * @param client_socket 
         * @return 
         */
        [[nodiscard]] std::vector<char> send(int client_socket) const ;

        /**
        * Return a vector of {@link SocketDescriptor}. Will return both an IPV6 and IPV4 address
        * if dual stack sockets is enabled.
        */
        [[nodiscard]] std::vector<SocketDescriptor> getSockets() const;

        void getAddresses(const std::string& address, SOCK_TYPE socketType, const std::string& port, SOCK_FAM family);



    };

}



