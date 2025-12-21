//
// Created by bripl on 12/18/2025.
//

#include "WinSockManager.h"
#include <iostream>

namespace Waiter::Networking {

    void WinSockManager::initialize() {
        static WinSockManager WinSockManager;
    }

    WinSockManager::WinSockManager() {

        #ifdef _WIN32
        WSADATA wsaData;
        int status = 0;

        // WSA version max version that can be used
        constexpr WORD version = MAKEWORD(2, 2); // Version 2.2

        // Find WSA DLL
        status = WSAStartup(version, &wsaData);

        // Check WSA errors
        if (status < 0) {
            std::cerr << "WSAStartup failed: " << status << std::endl;
            WSACleanup();
            throw std::runtime_error("WSAStartup failed: " + std::to_string(status));
        }

        // Check DLL version
        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
            std::cerr << "WSAStartup failed: wrong version" << std::endl;
            WSACleanup();
            throw std::runtime_error("WSAStartup failed: Wrong version.");
        }
        std::cout << "WSAStartup successful." << std::endl;
        #endif

    }

    WinSockManager::~WinSockManager() {
        #ifdef _WIN32
        WSACleanup();
        #endif
    }

}