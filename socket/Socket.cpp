//
// Created by b1n4r33 on 3/10/25.
//

#include "Socket.h"
#include <iostream>

#ifdef linux
    // Linux specific networking APIs
    #include <cstring>
    #include <ostream>
    #include <sys/socket.h>
    #include <fcntl.h>
#endif
#ifdef _WIN32
    // Windows socket APIs
    #include <winsock2.h>
    #include <ws2tcpip.h>

#endif

#ifdef linux

int Socket::handle_connection(int client_socket) const {
    // Number of bytes sent from new connection
    ssize_t bytes_read = 0;
    // Current connection status
    int status = 1;
    // Receive bytes until done
    while (status > 0) {
        char buffer[1024] ={};
        bytes_read = recv(client_socket, buffer, sizeof(buffer)-1, 0);
        std::cout << "Bytes read: " << bytes_read << std::endl;
        if (bytes_read < 0) {
            std::cerr << "Receive bytes failed: "<< strerror(errno) << std::endl;
            status = -1;
        }
        if (bytes_read == 0) {
            std::cout << "Connection closed" << std::endl;
            status = 0;
        }
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] != '\r') {
                std::cout << buffer[i];
            }
        }
        std::string message = "HTTP/1.0 200 OK\r\n\r\n<h1>Hello shawtyyyyy!</h1>";
        write(client_socket, message.c_str(), message.length());

    }
    // Close connection and return
    close(client_socket);
    return status;
}

int Socket::start() {
    this->sock = socket(AF_INET, protocol, 0);
    if (this->sock < 0) {
        std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
        return -1;
    }
    std::cout << "Socket successfully created" << std::endl;
    // Set socket to reuse address without waiting for downtime when debug is true
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &debug, sizeof(int));
    sock_address = sockaddr_in{};
    // Set socket address properties
    sock_address.sin_family = AF_INET;
    sock_address.sin_addr.s_addr = INADDR_ANY;
    sock_address.sin_port = htons(port);

    // Bind socket to address
    status = bind(sock, reinterpret_cast<struct sockaddr *>(&sock_address), sizeof(sock_address));

    if (status < 0) {
        std::cerr << "Socket bind failed: " << strerror(errno) << std::endl;
        return -1;
    }
    std::cout << "Socket successfully bound" << std::endl;
    status = listen(sock, SOMAXCONN);
    if (status < 0) {
        std::cerr << "Socket listen failed: " << strerror(errno) << std::endl;
        return -1;
    }
    std::cout << "Socket successfully listening" << std::endl;

    while (status >= 0) {
        std::cout << "Waiting for a new connection" << std::endl;
        const int new_connection = accept(sock, nullptr, nullptr);
        if (new_connection < 0) {
            std::cerr << "Socket accept failed: " << strerror(errno) << std::endl;
            return -1;
        }
        std::cout << "New connection accepted" << std::endl;
        status = handle_connection(new_connection);
    }
    return 0;

}

#endif

/*
 * Method implementations if in windows. Method implementations will depend on windows APIs
 */
#ifdef _WIN32

namespace Waiter::Networking {

    Socket::Socket(const SocketArgs args): buffer{}, sockArgs(args), sock(INVALID_SOCKET), sockAddr(SOCKADDR_STORAGE()),
    isReuseAddress(false), isNonBlocking(false), hints(), addressList(nullptr), wsaData(WSAData()) {
        // WSA version max version that can be used
        constexpr WORD version = MAKEWORD(2, 2); // Version 2.2
        status = WSAStartup(version, &wsaData);
        // Find WSA DLL
        if (status < 0) {
            std::cerr << "WSAStartup failed: " << status << std::endl;
            WSACleanup();
            throw std::runtime_error("WSAStartup failed: " + std::to_string(status));
        }
        // Check DLL version
        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
            std::cerr << "WSAStartup failed: wrong version" << std::endl;
            WSACleanup();
            throw std::runtime_error("WSAStartup failed: Wrong version.");
        }
        std::cout << "WSAStartup successful." << std::endl;
        // Set family for protocol independent socket address info
        sockAddr.ss_family = sockArgs.family;
        // Create socket descriptor
        sock = socket(args.family, args.type, args.protocol ? args.protocol : 0);
        if (sock == INVALID_SOCKET) {
            std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
            WSACleanup();
            throw std::runtime_error("Socket creation failed: " + WSAGetLastError());
        }
        std::cout << "Socket created successfully." << std::endl;
    }

    Socket::Socket(const SocketArgs args, std::nothrow_t): buffer{}, sockArgs(args), sock(INVALID_SOCKET),
    sockAddr(SOCKADDR_STORAGE()), isReuseAddress(false), isNonBlocking(false), hints(), addressList(nullptr), wsaData(WSAData()) {
        // WSA version max version that can be used
        constexpr WORD version = MAKEWORD(2, 2); // Version 2.2
        status = WSAStartup(version, &wsaData);
        // Find WSA DLL
        if (status < 0) {
            std::cerr << "WSAStartup failed: " << status << std::endl;
            WSACleanup();
            return;
        }
        // Check DLL version
        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
            std::cerr << "WSAStartup failed: wrong version" << std::endl;
            WSACleanup();
            return;
        }
        std::cout << "WSAStartup successful." << std::endl;
        // Set family for protocol independent socket address info
        sockAddr.ss_family = sockArgs.family;
        // Create socket descriptor. In windows socket is never negative. Not true for linux
        sock = socket(args.family, args.type, 0);
        if (sock == INVALID_SOCKET) {
            std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
            WSACleanup();
            return;
        }
        std::cout << "Socket created successfully." << std::endl;
    }


    int Socket::bindSocket(const std::string &address=DEFAULT_ADDRESS, const std::string &port=DEFAULT_PORT) {
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = sockArgs.family;
        hints.ai_socktype = sockArgs.type;
        hints.ai_flags = AI_NUMERICHOST | AI_PASSIVE;
        status = getaddrinfo(address.data(), port.data(), &hints, &addressList);
        if (status != 0) {
            fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(status));
            WSACleanup();
            throw std::runtime_error("getaddrinfo failed: " + std::to_string(status));
        }
        AddressInfo *addressPtr = addressList;
        for (int i = 0; addressPtr != nullptr; i++, addressPtr++) {
            if (i == FD_SETSIZE) {
                printf("bindSocket failed getting address info. Too many addresses returned.\n");
                break;
            }
            // Ignore address if family is not IPV4 or IPV6
            if (addressPtr->ai_family != PF_INET && addressPtr->ai_family != PF_INET6) {
                printf("Address family for found address not supported.\n");
                continue;
            }


        }

        return 0;

    }


    int Socket::listen() {


        return 0;
    }

    Socket::~Socket() {
        closesocket(sock);
        WSACleanup();
    }

    void Socket::setReuseAddress(const bool value){
        // SOL_SOCKET are protocol independent options
        status = setsockopt(sock, SOL_SOCKET SO_EXCLUSIVEADDRUSE, (char*) &)
        this->isReuseAddress = value;
    }

    void Socket::setNonBlocking(const bool value)
    {
        this->isNonBlocking = value;
    }

    SocketArgs Socket::getSockArgs() const { return this->sockArgs; }

    SocketDescriptor Socket::getSockDescriptor() const { return this->sock; }

}

#endif



