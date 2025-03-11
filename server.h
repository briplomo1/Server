//
// Created by b1n4r33 on 3/9/25.
//

#ifndef SERVER_H
#define SERVER_H

enum TRANSPORT {
    TCP = 0,
    UDP = 1
};

enum IP_VERSION {
    IP_V4 = 0,
    IP_V6 = 1,
};

enum HTTP_RESPONSE {
    HTTP_OK = 200,
    HTTP_CREATED = 201,
    HTTP_BAD_REQUEST = 400,
    HTTP_UNAUTHORIZED = 401,
    HTTP_FORBIDDEN = 403,
    HTTP_NOT_FOUND = 404,
    HTTP_METHOD_NOT_ALLOWED = 405,
    HTTP_INTERNAL_SERVER_ERROR = 500,
    HTTP_BAD_GATEWAY = 502,
    HTTP_SERVICE_UNAVAILABLE = 503,
    HTTP_GATEWAY_TIMEOUT = 504
};


class server {

private:
    short port;
    TRANSPORT protocol;
    IP_VERSION ip_version;

public:

    server(const short port, const TRANSPORT protocol, IP_VERSION ip_version) : port(port), protocol(protocol), ip_version(ip_version) {}

    ~server();

    int listen();

private:

    int handleConnection(int client_socket);


};



#endif //SERVER_H
