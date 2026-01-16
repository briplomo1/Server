//
// Created by bripl on 12/22/2025.
//

#ifndef SERVER_H
#define SERVER_H
#include "endpoint/Endpoint.h"


namespace Waiter::TCP {

    class Server {

        void bind(Endpoint::InetEndpoint endpoint);

        void listen(Endpoint::InetEndpoint endpoint);

        void accept(Endpoint::InetEndpoint endpoint);
    };

}




#endif //SERVER_H
