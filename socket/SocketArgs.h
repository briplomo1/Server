//
// Created by bripl on 3/19/2025.
//

#ifndef SOCKETARGS_H
#define SOCKETARGS_H

#ifdef linux
    #include <stdio.h>
#endif

#ifdef _WIN32

    #include <winsock2.h>
    #include <windows.h>

#endif
#include <vector>


enum SOCK_FAM {
    IPV4 = AF_INET,
    IPV6 = AF_INET6,
    BLUETOOTH =
    #ifdef linux
        AF_BLUETOOTH
    #endif
    #ifdef _WIN32
        AF_BTH,
    #endif
};

enum SOCK_TYPE {
    TCP = SOCK_STREAM,
    UDP = SOCK_DGRAM,
    RAW = SOCK_RAW,
};



struct SocketArgs {

    SOCK_FAM family;
    SOCK_TYPE type;
    bool nonblocking;
    bool reuseaddr;

    SocketArgs(const SOCK_FAM family, const SOCK_TYPE type, const bool nonblocking=false, const bool reuseAddress) : family(
        family), type(type), nonblocking(nonblocking), reuseaddr(reuseAddress) {
#ifdef linux
        // TODO: handle linux socket flags
#endif
    }
};



#endif //SOCKETARGS_H
