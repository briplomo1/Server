//
// Created by bripl on 1/16/2026.
//

#ifndef ENDPOINTOPTIONS_H
#define ENDPOINTOPTIONS_H
#include <waiter/types.h>

namespace Waiter {

    template<ADDR_FAM AddressFamily, bool IsServer, bool IsDualStack=true>
    struct EndpointOptions;

    template<bool IsServer>
    struct EndpointOptions<IPV4, IsServer, false> {
        static constexpr bool passive = IsServer;
        static constexpr bool addr_config = !IsServer;
        static constexpr bool v4_mapped = true;
        static constexpr bool numeric_port = true;
        bool canon_name = false;
    };

    template<bool IsServer, bool IsDualStack>
    struct EndpointOptions<IPV6, IsServer, IsDualStack> {
        static constexpr bool passive = IsServer;
        static constexpr bool addr_config = !IsServer;
        static constexpr bool v4_mapped = IsDualStack;
        static constexpr bool numeric_port = true;
        bool canon_name = false;
    };

    template<bool IsServer>
    struct EndpointOptions<UNIX, IsServer> {
        static constexpr bool passive = IsServer;
        // TODO:
    };

    template<bool IsServer>
    struct EndpointOptions<BLUETOOTH, IsServer> {
        static constexpr bool passive = IsServer;
        // TODO:
    };




}

#endif //ENDPOINTOPTIONS_H
