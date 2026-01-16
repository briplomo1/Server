//
// Created by bripl on 12/28/2025.
//

#ifndef ENDPOINT_H
#define ENDPOINT_H
#include <cstdint>
#include <string>
#include <ws2tcpip.h>
#include <stdexcept>
#include <cstring>
#include <variant>
#include "../socket/SocketTypes.h"

#define DEFAULT_ADDRESS "127.0.0.1"
#define DEFAULT_PORT 8089


namespace Waiter::Endpoint {

    template<ADDR_FAM AddressFamily>
    concept InetAddress = AddressFamily == IPV4 || AddressFamily == IPV6 || AddressFamily == IP_ANY;

    template<ADDR_FAM AddressFamily>
    concept UnixAddress = AddressFamily == UNIX;

    template<ADDR_FAM AddressFamily>
    concept BluetoothAddress = AddressFamily == BLUETOOTH;

    template<ADDR_FAM AddressFamily, bool IsServer>
    struct EndpointOptions {
        static constexpr bool passive = IsServer;
    };

    template<ADDR_FAM AddressFamily, SOCK_TYPE SocketType, PROTOCOL Protocol, IP_TYPE IPType, bool IsServer>
    class Endpoint {

        AddressStorage address{};

        socklen_t address_len{};

    public:

        const sockaddr* addr() const { return reinterpret_cast<const sockaddr*>(&address); }

        socklen_t addr_len() const { return address_len; }

        Endpoint(const std::string& host, const short port, EndpointOptions<IPV4, IsServer> opts)
            requires InetAddress<AddressFamily> {
            resolve(std::move(host), port, opts);
        }

        explicit Endpoint(const std::string& path) requires UnixAddress<AddressFamily> {

        }

        explicit Endpoint(short unsigned bluetooth_channel, const std::string& device_addr)
            requires BluetoothAddress<AddressFamily> {

        }

    private:

        /**
         * Determines if host string is numeric ip in the form: 127.0.0.1 versus 'localhost'.
         * @param s  The host string
         * @return bool representing if the host string is numeric.
         */
        static bool isNumericIP(const std::string& s)
            requires InetAddress<AddressFamily> {
            if constexpr (AddressFamily == AF_INET) {
                in_addr addr{};
                return inet_pton(AddressFamily, s.c_str(), &addr) == 1;
            } else if constexpr (AddressFamily == AF_INET6) {
                in6_addr addr{};
                return inet_pton(AddressFamily, s.c_str(), &addr) == 1;
            }
            else {
                return false;
            }
        }

        void resolve(const std::string& host, const short port, EndpointOptions<IPV4, IsServer> opts)
        requires InetAddress<AddressFamily> {
            AddressInfo * addr_list = nullptr;
            AddressInfo hints = {};
            memset(&hints, 0, sizeof(hints));
            hints.ai_family = IPV4;
            hints.ai_socktype = SocketType;
            hints.ai_protocol = Protocol;
            hints.ai_flags = IsServer ? AI_PASSIVE : 0;

            if constexpr (AddressFamily == AF_INET6) {
                if constexpr (IPType == IP_TYPE::DUAL_STACK) {
                    hints.ai_flags |= AI_V4MAPPED;
                }
            }

            const std::string portStr = std::to_string(port);
            if (const int res = getaddrinfo(host.c_str(), portStr.c_str(), &hints, &addr_list); res != 0) {
                throw std::runtime_error("getaddrinfo() failed: " + std::string(gai_strerror(res)));
            }

            if (!addr_list) {
                throw std::runtime_error("getaddrinfo() failed: No address found: " + std::string(gai_strerror(errno)));
            }

            memset(&address, 0, sizeof(address));
            std::memcpy(&address, addr_list->ai_addr, addr_list->ai_addrlen);
            address_len = addr_list->ai_addrlen;

            freeaddrinfo(addr_list);
        }



    };


    };


}

#endif //ENDPOINT_H
