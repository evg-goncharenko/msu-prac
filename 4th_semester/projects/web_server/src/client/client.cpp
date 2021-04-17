#include "client.h"
#include <sys/socket.h>

// class ClientSocket:

void ClientSocket::to_connect(const SocketAddress& serverAddr) {
    if (connect(sd_, serverAddr.get_addr(), serverAddr.get_addr_len()) < 0) {
        std::cout << "Error: Can't connect in ClientSocket" << std::endl;
    }
}

//class HttpHeader:

HttpHeader::HttpHeader(const HttpHeader& copy) {
    name_ = copy.name_;
    value_ = copy.value_;
}

std::string HttpHeader::string_concat() const {
    return name_ + value_;
}

HttpHeader HttpHeader::parse_header(const std::string& line) {    
   int i = 0;
    std::string new_name, new_value;
    if (!line.empty()){
        while (line[i] != ' ') {
            new_name += line[i];
            i++;
        }
        new_name += '\0';
        
        while (i < line.size()) {
            new_value += line[i];
            i++;
        }
        new_value += '\0';
    } else {
        new_name = " "; new_value = " ";
    }
    HttpHeader temp(new_name, new_value);
    return temp;
}

//class HttpRequest:
    
HttpRequest::HttpRequest() {
    lines_ = {"GET /f.txt HTTP/1.1"};
}

std::string HttpRequest::string_concat() const {
    std::string res;
    for (auto i = 0; i < lines_.size(); ++i) {
        res += lines_[i];
    }
    return res;
}

// class HttpResponse: 

HttpResponse::HttpResponse(std::vector<std::string> lines) {
    response_ = HttpHeader::parse_header(lines[0]);
    other_ = new HttpHeader[lines.size() - 1];
    int i;
    for (i = 1; i < lines.size(); i++) {
        other_[i - 1] = HttpHeader::parse_header(lines[i]);
        if ((lines[i]).empty()) {
            body_ = lines[i + 1];
            break;
        }
    }
    length_ = i;
}

void HttpResponse::print_all() const {
    std::cout << CYAN_COLOR << "response_ : " << RESET_COLOR << response_.string_concat() << "'" << std::endl;
    int j = 0;
    while (j < length_) {
        std::cout << CYAN_COLOR << "other[" << j << "] : " << RESET_COLOR << (other_[j]).string_concat() << std::endl;
        j++;
    }
    std::cout << std::endl;
    std::cout << CYAN_COLOR << "Start body_ : " << RESET_COLOR << std::endl;
    std::cout << body_ << std::endl;
    std::cout << CYAN_COLOR << "End body_" << RESET_COLOR << std::endl;
}

HttpResponse::~HttpResponse() {
    delete[] other_;
}

void client_connection() {
    std::cout << std::endl;
    ClientSocket s;
    SocketAddress saddr(BASE_ADDR, PORT);
    s.to_connect(saddr);

    // Make request:
    HttpRequest rq;
    std::string req = rq.string_concat();
    s.to_write(req);
    std::vector<std::string> lines;
    std::string str_responce;
    std::string tmp;
    for (auto i = 0; i < 3; ++i) {
        s.to_read(str_responce);
        tmp += str_responce;
    }
    lines = split_lines(tmp);
    HttpResponse resp(lines);
    resp.print_all();
    s.shutting_down();
}