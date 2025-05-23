//
// Created by b1n4r33 on 3/10/25.
//

#include "Socket.h"
#include <iostream>
#include <utility>

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

#ifdef linux

namespace Waiter::Networking
{

    Socket::Socket(std::string address, const SOCK_FAM addressFamily, const SOCK_TYPE socketType, const SOCK_PROTOCOL socketProtocol=DEFAULT):
    addressFamily(addressFamily), socketType(socketType), protocol(socketProtocol), address(std::move(address)), addresses(std::vector<AddressInfo>()),
    sockets(std::vector<SocketDescriptor>()), connStorage(SOCKADDR_STORAGE())
    {
        // TODO: Get IPV6 and IPV4 addresses via getAddresses
        // TODO: Create IPV6 and IPV4 sockets via getSockets
    }

    Socket::Socket(std::string address, const SOCK_FAM addressFamily, const SOCK_TYPE socketType, const SOCK_PROTOCOL socketProtocol=DEFAULT, std::nothrow_t):
    addressFamily(addressFamily), socketType(socketType), protocol(socketProtocol), address(std::move(address)), addresses(std::vector<AddressInfo>()),
    sockets(std::vector<SocketDescriptor>()), connStorage(SOCKADDR_STORAGE())
    {
        // TODO: Get IPV6 and IPV4 addresses via getAddresses
        // TODO: Create IPV6 and IPV4 sockets via getSockets
    }

    void Socket::bindSocket(const std::string &address, const std::string &port)
    {
        // TODO: Bind socket

    }

    std::vector<SocketDescriptor> Socket::getSockets() const
    {
        // TODO: Get sockets
    }


    Socket::~Socket()
    {
        for (const SocketDescriptor sock: sockets)
        {
            closesocket(sock);
        }

    }


}

#endif

/*
 * Method implementations if in windows. Method implementations will depend on windows APIs
 */
#ifdef _WIN32

namespace Waiter::Networking {

    Socket::Socket(std::string address, const SOCK_FAM addressFamily, const SOCK_TYPE socketType, const SOCK_PROTOCOL socketProtocol=DEFAULT):
    addressFamily(addressFamily), socketType(socketType), protocol(socketProtocol), address(std::move(address)), addresses(std::vector<AddressInfo>()),
    sockets(std::vector<SocketDescriptor>()), connStorage(SOCKADDR_STORAGE()), wsaData(WSAData()) {

        int status = 0;
        // WSA version max version that can be used
        constexpr WORD version = MAKEWORD(2, 2); // Version 2.2
        status = WSAStartup(version, &wsaData);
        // Find WSA DLL
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

        // Set family for protocol independent incoming socket connections
        connStorage.ss_family = addressFamily;

        // TODO: get IPV4 and IPV6 addresses
        // TODO: create sockets

        // Set family for protocol independent socket address info
        // sockAddr.ss_family = sockArgs.family;
        // // Create socket descriptor
        // sock = socket(args.family, args.type, args.protocol ? args.protocol : 0);
        // if (sock == INVALID_SOCKET) {
        //     std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        //     WSACleanup();
        //     throw std::runtime_error("Socket creation failed: " + WSAGetLastError());
        // }
        // std::cout << "Socket created successfully." << std::endl;
    }

    Socket::Socket(std::string address, const SOCK_FAM addressFamily, const SOCK_TYPE socketType, const SOCK_PROTOCOL socketProtocol=DEFAULT, std::nothrow_t):
    addressFamily(addressFamily), socketType(socketType), protocol(socketProtocol), address(std::move(address)), addresses(std::vector<AddressInfo>()),
    sockets(std::vector<SocketDescriptor>()), connStorage(SOCKADDR_STORAGE()), wsaData(WSAData()) {

        int status = 0;
        // WSA version max version that can be used
        constexpr WORD version = MAKEWORD(2, 2); // Version 2.2
        status = WSAStartup(version, &wsaData);

        // Find WSA DLL
        if (status < 0) {
            std::cerr << "WSAStartup failed: " << status << std::endl;
            WSACleanup();
            return;
        }

        // Check DLL version
        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
            std::cerr << "WSAStartup failed: wrong version" << std::endl;
            WSACleanup();
            return;
        }
        std::cout << "WSAStartup successful." << std::endl;

        // Set family for protocol independent incoming socket connections
        connStorage.ss_family = addressFamily;

        // Create socket descriptor. In windows socket is never negative. Not true for linux
        // sock = socket(args.family, args.type, 0);
        // if (sock == INVALID_SOCKET) {
        //     std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        //     WSACleanup();
        //     return;
        // }
        // std::cout << "Socket created successfully." << std::endl;
    }


    void Socket::bindSocket(const std::string &address=DEFAULT_ADDRESS, const std::string &port=DEFAULT_PORT) {
        int status = 0;
        // for (int i = 0; addressPtr != nullptr; i++, addressPtr++) {
        //     if (i == FD_SETSIZE) {
        //         printf("bindSocket failed getting address info. Too many addresses returned.\n");
        //         break;
        //     }
        //     // Ignore address if family is not IPV4 or IPV6
        //     if (addressPtr->ai_family != PF_INET && addressPtr->ai_family != PF_INET6) {
        //         printf("Address family for found address not supported.\n");
        //         continue;
        //     }
        //
        //
        // }

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
     * @param port A string port to connect the socket on
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
        for (const SocketDescriptor sock: sockets) {
            closesocket(sock);
        }
        WSACleanup();
    }

    std::vector<SocketDescriptor> Socket::getSockets() const { return this->sockets; }

}

#endif



