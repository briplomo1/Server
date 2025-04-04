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

        // Socket properties defined by child classes. Defines the type of connection
        SOCK_FAM addressFamily;
        SOCK_TYPE socketType;
        SOCK_PROTOCOL protocol = DEFAULT;

        // User defined socket connection properties
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
        // Non-implicit conversion constructor
        explicit Socket(std::string address, SOCK_FAM addressFamily, SOCK_TYPE socketType, SOCK_PROTOCOL socketProtocol);

        // Non exception throwing constructor. Terminates process on failure
        Socket(std::string address, SOCK_FAM addressFamily, SOCK_TYPE socketType, SOCK_PROTOCOL socketProtocol, std::nothrow_t);

        /**
         * Clean up resources. Will do common resource closing such as closing socket.
         * Protocol and socket type will define further resource cleanup.
         */
        virtual ~Socket();

        /**
         * Bind a socket to an IP address and port. Uses {@link SOCK_ADDRESS} and a {@link port} to bind.
         */
        int bindSocket(const std::string &address, const std::string &port);


        /**
         * Calling application will define socket type/protocol on which the listen implementation will depend.
         * @return
         */
        virtual int listen() = 0;

        /**
        * Return a vector of {@link SocketDescriptor}. Will return both an IPV6 and IPV4 address
        * if dual stack sockets is enabled.
        */
        [[nodiscard]] std::vector<SocketDescriptor> getSockets() const;

    private:

        void getAddresses(const std::string& address, SOCK_TYPE socketType, const std::string& port, SOCK_FAM family);

        //[[nodiscard]] int handleReceive(int client_socket) const ;

        //[[nodiscard]] int handleSend(int client_socket) const ;



    };

}


#endif //SOCKET_H
