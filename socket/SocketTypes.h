//
// Created by bripl on 3/19/2025.
//

#pragma once

#ifdef linux
    #include <stdio.h>
#endif

#ifdef _WIN32
    // Windows specific socket API
    #include <winsock2.h>
#endif

#include <ws2tcpip.h>


namespace Waiter {

    using AddressStorage = sockaddr_storage;
    using SocketDescriptor = SOCKET;
    using AddressInfo = addrinfo;

    /**
     * The underlying networking protocol.
     */
    enum ADDR_FAM {
        IPV4 = AF_INET,
        IPV6 = AF_INET6,
        IP_ANY = AF_UNSPEC,
        UNIX = AF_UNIX,
        BLUETOOTH =
        #ifdef linux
            AF_BLUETOOTH // Bluetooth socket type exclusive to linux
        #endif
        #ifdef _WIN32
            AF_BTH, // Windows bluetooth
        #endif
    };

    /**
     * The socket type. Defines communication protocol.
     */
    enum SOCK_TYPE: int {
        TCP = SOCK_STREAM,
        UDP = SOCK_DGRAM,
        RAW = SOCK_RAW,
    };

    /**
     * The socket transport protocol. Default is TCP for {@link SOCK_STREAM} and UDP for {@link SOCK_DGRAM}
     * and will mostly be interchangeable, however there are exceptions outside the scope of this library.
     */
    enum PROTOCOL : int {
        DEFAULT = 0,
    };

    enum class IP_TYPE: int {
        IPV4_ONLY,
        IPV6_ONLY,
        DUAL_STACK,
    };

    enum class SocketResult {

    };

}

