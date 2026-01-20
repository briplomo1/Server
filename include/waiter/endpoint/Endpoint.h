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
#include <waiter/endpoint/EndpointOptions.h>
#include <waiter/types.h>
#include <waiter/io/WinSockManager.h>

#define DEFAULT_ADDRESS "127.0.0.1"
#define DEFAULT_PORT 8089


namespace Waiter {

    template<ADDR_FAM AddressFamily, SOCK_TYPE SocketType, PROTOCOL Protocol, bool IsServer, bool IsDualStack=false>
    class Endpoint {

        using Opts = EndpointOptions<AddressFamily, IsServer, IsDualStack>;

        AddressStorage address{};

        socklen_t address_len{};

    public:

        const sockaddr* addr() const { return reinterpret_cast<const sockaddr*>(&address); }

        [[nodiscard]] socklen_t addr_len() const { return address_len; }

        explicit Endpoint(const std::string& host=DEFAULT_ADDRESS, const short port=DEFAULT_PORT, Opts opts=Opts{})
            requires (AddressFamily == IP_ANY || AddressFamily == IPV4 || AddressFamily == IPV6) {
            WinSockManager::initialize();
            resolve_inet(std::move(host), port, opts);
        }

        explicit Endpoint(const std::string& path) requires (AddressFamily == UNIX) {
            build_unix(std::move(path));
        }

        Endpoint(short unsigned bluetooth_channel, const std::string& device_addr)
            requires (AddressFamily == BLUETOOTH) {
            build_bluetooth(std::move(bluetooth_channel), device_addr);
        }

    private:

        /**
         * Determines if host string is numeric ip in the form: 127.0.0.1 versus 'localhost'.
         * @param s  The host string
         * @return bool representing if the host string is numeric.
         */
        bool isNumericIP(const std::string& s)
            requires (AddressFamily == IP_ANY || AddressFamily == IPV4 || AddressFamily == IPV6) {
            if constexpr (AddressFamily == IPV4) {
                in_addr addr{};
                return inet_pton(AddressFamily, s.c_str(), &addr) == 1;
            } else if constexpr (AddressFamily == IPV6) {
                in6_addr addr{};
                return inet_pton(AddressFamily, s.c_str(), &addr) == 1;
            }
            else {
                return false;
            }
        }

        void resolve_inet(const std::string& host, const short port, Opts opts)
        requires (AddressFamily == IP_ANY || AddressFamily == IPV4 || AddressFamily == IPV6) {
            AddressInfo * addr_list = nullptr;
            AddressInfo hints = {};
            memset(&hints, 0, sizeof(hints));

            hints.ai_family = IPV4;
            hints.ai_socktype = SocketType;
            hints.ai_protocol = Protocol;
            hints.ai_flags = 0;

            // Set given and compiled options
            if constexpr (EndpointOptions<AddressFamily, IsServer, IsDualStack>::passive)      hints.ai_flags |= AI_PASSIVE;
            if constexpr (EndpointOptions<AddressFamily, IsServer, IsDualStack>::addr_config)  hints.ai_flags |= AI_ADDRCONFIG;
            if constexpr (EndpointOptions<AddressFamily, IsServer, IsDualStack>::v4_mapped)    hints.ai_flags |= AI_V4MAPPED;
            if constexpr (EndpointOptions<AddressFamily, IsServer, IsDualStack>::numeric_port) hints.ai_flags |= AI_NUMERICSERV;
            if (opts.canon_name) hints.ai_flags |= AI_CANONNAME;
            if (isNumericIP(host)) hints.ai_flags |= AI_NUMERICHOST;


            const std::string portStr = std::to_string(port);
            if (const int res = getaddrinfo(host.c_str(), portStr.c_str(), &hints, &addr_list); res != 0) {
                throw std::runtime_error("getaddrinfo() failed: " + std::string(gai_strerror(res)));
            }

            if (!addr_list) {
                throw std::runtime_error("getaddrinfo() failed: No address found: " + std::string(gai_strerror(errno)));
            }

            std::memcpy(&address, addr_list->ai_addr, addr_list->ai_addrlen);
            address_len = addr_list->ai_addrlen;

            freeaddrinfo(addr_list);
        }

        void build_unix(const std::string& path)
            requires (AddressFamily == UNIX) {
                // TODO:
            // sockaddr sa{};
            // sa.sun_family = UNIX;
            // std::strncpy(sa.sun_path, path.c_str(), sizeof(sa.sun_path) - 1);
            //
            // std::memcpy(&address, &sa, sizeof(sa));
            // address_len = sizeof(sockaddr_un);
        }

        void build_bluetooth(unsigned short channel, const std::string& device)
            requires (AddressFamily == BLUETOOTH) {
            // TODO
        }

    };


}

#endif //ENDPOINT_H
