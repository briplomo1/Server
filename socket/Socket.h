//
// Created by b1n4r33 on 3/10/25.
//

#ifndef SOCKET_H
#define SOCKET_H

#include "SocketArgs.h"
#include <cstdint>
#include <new>

/**
 * What properties and methods are common to all socket and protocol types will be implemented in this SScoetk class.
 * Specific protocols and socket types will extend virtual methods
 */
namespace Waiter::Networking {

    typedef uint64_t SocketDescriptor;

    class Socket {
        // Arguments for defining and creating socket
        SocketArgs sockArgs;
        // unsigned 64 bit for modularity with win and *nix archs
        SocketDescriptor sock;
        // IP version agnostic structure holds socket address information
        SOCKADDR_STORAGE sockAddr;

        int status;

        bool isReuseAddress;

        bool isNonBlocking;

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
         * Bind a socket to a full address and port. Uses {@link SocketArgs} and a {@link port} to bind.
         */
        int bindSocket(int port);

        /**
         * Calling application will define socket type/protocol on which the listen implementation will depend.
         * @return
         */
        virtual int listen() = 0;

    private:

        [[nodiscard]] int handleReceive(int client_socket) const ;

        [[nodiscard]] int handleSend(int client_socket) const ;

        void setReuseAddress(bool value);

        void setNonBlocking(bool value);

        [[nodiscard]] SocketArgs getSockArgs() const ;

        [[nodiscard]] SocketDescriptor getSockDescriptor() const ;

    };

}


#endif //SOCKET_H
