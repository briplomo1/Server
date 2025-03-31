//
// Created by bripl on 3/19/2025.
//

#ifndef SOCKETARGS_H
#define SOCKETARGS_H

#ifdef linux
    #include <stdio.h>
#endif

#ifdef _WIN32
    // Windows specific socket API
    #include <winsock2.h>
    #include <windows.h>

#endif
#include <string>


namespace Waiter::Networking {

    enum SOCK_FAM {
        IPV4 = AF_INET,
        IPV6 = AF_INET6,
        UNSPECIFIED = AF_UNSPEC,
        BLUETOOTH =
        #ifdef linux
            AF_BLUETOOTH // Bluetooth socket type exclusive to linux
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

    enum SOCK_PROTOCOL {
        DEFAULT = 0,
    };


    struct SocketArgs {

        SOCK_FAM family;
        SOCK_TYPE type;
        SOCK_PROTOCOL protocol;
        std::string port;

        SocketArgs(const SOCK_FAM family, const SOCK_TYPE type, const SOCK_PROTOCOL protocol, const std::string &port) : family(
            family), type(type), protocol(protocol), port(port) {}

    };



}

#endif //SOCKETARGS_H
