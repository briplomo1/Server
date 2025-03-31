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
#include <ws2tcpip.h>

/**
 * What properties and methods are common to all socket and protocol types will be implemented in this SScoetk class.
 * Specific protocols and socket types will extend virtual methods
 */
namespace Waiter::Networking {

    typedef addrinfo AddressInfo;
    typedef uint64_t SocketDescriptor;


    class Socket {
        char buffer[BUFFER_SIZE];
        // Arguments for defining and creating socket
        SocketArgs sockArgs;
        // unsigned 64 bit for modularity with win and *nix archs
        SocketDescriptor sockets[];
        // IP version agnostic structure holds socket address information
        SOCKADDR_STORAGE sockAddr;

        int status;

        bool isReuseAddress;

        bool isNonBlocking;
        /**
         * Hints used to get host addresses
         */
        AddressInfo hints;
        /**
         * Ptr to list of addresses found for host by call to {@link getaddrinfo}.
         */
        AddressInfo *addressList;



#ifdef _WIN32
        WSAData wsaData; // Use WSA DLL data when in windows
#endif

    public:
        // Non-implicit conversion constructor
        explicit Socket(SocketArgs args);

        // Non exception throwing constructor. Terminates process on failure
        Socket(SocketArgs args, std::nothrow_t);

        /**
         * Clean up resources. Will do common resource closing such as closing socket.
         * Protocol and socket type will define further resource cleanup.
         */
        virtual ~Socket();

        /**
         * Bind a socket to an IP address and port. Uses {@link SocketArgs} and a {@link port} to bind.
         */
        int bindSocket(const std::string &address, const std::string &port);

        /**
         * Calling application will define socket type/protocol on which the listen implementation will depend.
         * @return
         */
        virtual int listen() = 0;

        void setReuseAddress(bool value);

        void setNonBlocking(bool value);

        [[nodiscard]] SocketArgs getSockArgs() const ;

        [[nodiscard]] SocketDescriptor getSockDescriptor() const ;

    private:

        [[nodiscard]] int handleReceive(int client_socket) const ;

        [[nodiscard]] int handleSend(int client_socket) const ;



    };

}


#endif //SOCKET_H
