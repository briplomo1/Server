//
// Created by b1n4r33 on 3/10/25.
//

#include "Socket.h"
#include <iostream>
#include "WinSockManager.h"

#ifdef linux
    // Linux specific networking APIs
    #include <cstring>
    #include <ostream>
    #include <sys/socket.h>
    #include <fcntl.h>
#endif
#ifdef _WIN32
    // Windows socket APIs
    #include <winsock2.h>
    #include <ws2tcpip.h>

#endif


namespace Waiter::Networking {

    Socket::Socket(
        const SOCK_FAM addressFamily,
        const SOCK_TYPE socketType,
        const SOCK_PROTOCOL socketProtocol=DEFAULT,
        std::nothrow_t):
    addressFamily(addressFamily),
    socketType(socketType),
    socketProtocol(socketProtocol) {

        // Singleton initialize WinSock
        WinSockManager::initialize();

        // Initialize socket descriptor
        SocketDescriptor socket = socket(addressFamily, socketType, socketProtocol);

        // Check for socket error
        if (socket == INVALID_SOCKET) {
            std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
            return;
        }
        std::cout << "Socket created" << std::endl;
    }

    /**
     * Makes use of {@link getaddrinfo} to retrieve all sockets by specifying the {@link SOCK_FAM}, {@link SOCK_TYPE}, port,
     * and network address. The system
     * @param family the {@link SOCK_FAM} to use as hint to retrieve network addresses. Setting to {@link IP_UNSPECIFIED}
     * will populate {@link addresses} with the IPV4 and IPV6 equivalents of the given address, thereby using a dual stack sockets.
     * Internally will use IPV6 addresses with a mapping to a IPV4 address when using IPV4 socket addresses.
     * Using other {@link SOCK_FAM} such as {@link IPV4} returns a more specific and single socket.
     * @param address A string representing the address to use for a socket
     * @param socketType The type of socket that is desired
     * @param port A string port to connect the socket on {}
     */
    void Socket::getAddresses(const std::string &address, const SOCK_TYPE socketType, const std::string &port, const SOCK_FAM family=IPV4) {
        AddressInfo hints = {};
        AddressInfo *addressesReturn;
        hints.ai_family = family;
        hints.ai_socktype = socketType;
        hints.ai_flags = AI_NUMERICHOST | AI_PASSIVE; // TODO: AI_PASSIVE only for server
        int status = 0;
        status = getaddrinfo(address.data(), port.data(), &hints, &addressesReturn) < 0;
        if (status != 0) {
            std::cerr << "Get addresses for supplied address hints failed: "<< status << gai_strerror(status)  << std::endl;
            throw std::runtime_error("getaddrinfo failed: " + std::to_string(status));
        }

        while (addressesReturn) {
            addresses.emplace_back(*addressesReturn);
            std::cout << "Address family: " << addressesReturn->ai_family << "\nAddress: " << addressesReturn->ai_addr
            << "\nSocket type: " << addressesReturn->ai_socktype << "\nProtocol: " << addressesReturn->ai_protocol << std::endl;
            addressesReturn = addressesReturn->ai_next;
        }
        std::cout << "Addresses successfully found: " << addresses.size() << std::endl;
    }

    Socket::~Socket() {
        #ifdef _WIN32
        shutdown(socketDescriptor, SD_BOTH);
        closesocket(socketDescriptor);
        #else
        close(socketDescriptor);
        shutdown(socketDescriptor, SHUT_RDWR);
        #endif
    }

}




