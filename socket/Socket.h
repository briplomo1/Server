//
// Created by b1n4r33 on 3/10/25.
//

#ifndef SOCKET_H
#define SOCKET_H

#define BUFFER_SIZE 1028
#define DEFAULT_ADDRESS "127.0.0.1"
#define DEFAULT_PORT "8000"

#include "SocketArgs.h"
#include <cstdint>
#include <memory>
#include <new>
#include <vector>
#include <ws2tcpip.h>

/**
 * What properties and methods are common to all socket and protocol types will be implemented in this SScoetk class.
 * Specific protocols and socket types will extend virtual methods
 */
namespace Waiter::Networking {

    typedef addrinfo AddressInfo;
    typedef uint64_t SocketDescriptor;

    class Socket {

        std::vector<char> recvBuffer; // Buffer to received data
        std::vector<char> sendBuffer; // Buffer for send data

        // User defined socket connection properties
        SOCK_FAM addressFamily;
        SOCK_TYPE socketType;
        SOCK_PROTOCOL protocol = DEFAULT;
        SOCK_ADDRESS address;
        SOCK_PORT port;

        /**
         * Vector of addresses found for host by call to {@link getaddrinfo}.
         * Will contain IPV4 and IPV6 addresses if dull stack sockets is enabled.
         * May contain more addresses if host has more than one network interface.
         */
        std::vector<AddressInfo> addresses;
        // unsigned 64 bit for modularity with win and *nix architectures
        std::vector<SocketDescriptor> sockets;
        // IP version agnostic structure specifies transport address for incoming connections
        SOCKADDR_STORAGE connStorage;

#ifdef _WIN32
        WSAData wsaData; // Use WSA DLL data when in windows
#endif

    public:
        /**
         * 
         * @param address 
         * @param addressFamily 
         * @param socketType 
         * @param socketProtocol 
         */
        Socket(std::string address, SOCK_FAM addressFamily, SOCK_TYPE socketType, SOCK_PROTOCOL socketProtocol);

        /**
         * Non exception throwing constructor. Will terminate process on failure.
         * @param address 
         * @param addressFamily 
         * @param socketType 
         * @param socketProtocol 
         */
        Socket(std::string address, SOCK_FAM addressFamily, SOCK_TYPE socketType, SOCK_PROTOCOL socketProtocol, std::nothrow_t);

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
         * Specific to server socket
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


#endif //SOCKET_H
