//
// Created by bripl on 12/18/2025.
//
#pragma once

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
#endif


#ifndef WINSOCKMANAGER_H
#define WINSOCKMANAGER_H



namespace Waiter {

    class WinSockManager {

        public:
            static void initialize();

        private:
            WinSockManager();
            ~WinSockManager();

            WinSockManager(const WinSockManager &) = delete;
            WinSockManager &operator=(const WinSockManager &) = delete;
    };

}

#endif //WINSOCKMANAGER_H
