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

    /**
     * The underlying networking protocol.
     */
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

    /**
     * The socket type. Defines communication protocol.
     */
    enum SOCK_TYPE {
        TCP = SOCK_STREAM,
        UDP = SOCK_DGRAM,
        RAW = SOCK_RAW,
    };

    /**
     * The socket transport protocol. Default is TCP for {@link SOCK_STREAM} and UDP for {@link SOCK_DGRAM}
     * and will mostly be interchangeable, however there are exceptions outside the scope of this library.
     */
    enum SOCK_PROTOCOL {
        DEFAULT = 0,
    };

    typedef std::string SOCK_ADDRESS;

    typedef std::string SOCK_PORT;

}

#endif //SOCKETARGS_H
