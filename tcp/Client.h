//
// Created by bripl on 12/22/2025.
//

#ifndef CLIENT_H
#define CLIENT_H
#include "endpoint/Endpoint.h"
#include "socket/BaseSocket.h"


namespace Waiter::TCP {

    class ClientSocket: BaseSocket {

        explicit ClientSocket(IP_TYPE type = IP_TYPE::DUAL_STACK);

        void connect(Endpoint::InetEndpoint endpoint);

    };

}



#endif //CLIENT_H
