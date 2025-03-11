//
// Created by b1n4r33 on 3/10/25.
//

#ifndef SOCKET_H
#define SOCKET_H
#include <unistd.h>
#include <vector>
#include <netinet/in.h>

enum SOCKET_FLAGS {
    NON_BLOCKING=SOCK_NONBLOCK,

};

enum SOCKET_PROTOCOL {
    TCP=SOCK_STREAM,
    UDP=SOCK_DGRAM
};

class Socket {
    int sock = 0;
    int sock_flags = 0;
    int protocol;
    int port;
    int debug;
    int status;
    sockaddr_in sock_address;

public:
    Socket(const SOCKET_PROTOCOL protocol, const int port, const std::vector<SOCKET_FLAGS>& flags = std::vector<SOCKET_FLAGS>(), const bool debug=false): protocol(protocol),
        port(port), debug(debug), status(0), sock_address() {
        for (const int flag: flags) {
            sock_flags |= flag;
        }
    }

    ~Socket() {
        close(sock);
    }

    int start();

private:

    int handle_connection(int client_socket) const ;

};



#endif //SOCKET_H
