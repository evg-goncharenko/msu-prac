#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

#include "../server/server.h"

class ClientSocket : public ConnectedSocket {
public:
    void to_connect(const SocketAddress& serverAddr);
};

class HttpHeader {
    std::string name_;
    std::string value_;

public:
    HttpHeader() = default;
    HttpHeader(const std::string& n, const std::string& v) : name_(n), value_(v) {}  // server <=> ResponceHeader
    HttpHeader(const HttpHeader& copy);
    std::string string_concat() const;
    static HttpHeader parse_header(const std::string& line);
};

class HttpRequest {
    std::vector<std::string> lines_;

public:
    HttpRequest();
    std::string string_concat() const;
};

class HttpResponse {
    HttpHeader response_;
    HttpHeader* other_;
    std::string body_;
    int length_;

public:
    HttpResponse(std::vector<std::string> lines);
    ~HttpResponse();
    void print_all() const;
};

void client_connection();

#endif  // CLIENT_H