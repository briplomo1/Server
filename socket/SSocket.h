//
// Created by b1n4r33 on 3/10/25.
//

#ifndef SOCKET_H
#define SOCKET_H

#include "SocketArgs.h"
#include <stdint.h>


class SSocket {

    SocketArgs args;
    // unsigned 64 bit for modularity with win and *nix archs
    uint64_t sock;
    short port;
    int status;
    #ifdef _WIN32
    WSAData wsaData;
    #endif

public:
    // Non-implicit conversion constructor
    explicit SSocket(SocketArgs args);

    // Non exception throwing constructor
    SSocket(SocketArgs args, std::nothrow_t);

    ~SSocket();

    int bind(int port);

    virtual int listen() = 0;

private:

    [[nodiscard]] int handleReceive(int client_socket) const ;

};


#endif //SOCKET_H
