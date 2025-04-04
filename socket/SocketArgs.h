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
        IP_UNSPECIFIED = AF_UNSPEC,
        IP_DUAL_STACK = PF_UNSPEC,
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

    typedef std::string SOCK_ADDRESS;

    typedef std::string SOCK_PORT;


}

#endif //SOCKETARGS_H
