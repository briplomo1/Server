//
// Created by b1n4r33 on 3/10/25.
//

#ifndef RESPONSE_H
#define RESPONSE_H
#include <map>
#include <string>
#include <utility>
#include <vector>

namespace HTTP {

    typedef std::map<std::string, std::string> Headers;
    typedef std::pair<std::string, std::string> Header;

    class Response {
        int statusCode;
        std::string body;
        Headers headers;

        public:
        Response(const int statusCode, std::string body, Headers  headers) : statusCode(statusCode), body(std::move(body)), headers(std::move(headers)) {}

        ~Response() {}

        void setStatusCode(const int statusCode) { this->statusCode = statusCode; }

        void setHeader(Header header);

        void setBody(std::string body) { this->body = std::move(body); }

        [[nodiscard]] std::string getBody() const { return this->body; }

        [[nodiscard]] Headers getHeaders() const { return this->headers; }

        [[nodiscard]] int getStatusCode() const { return this->statusCode; }


    };

} // HTTP

#endif //RESPONSE_H
