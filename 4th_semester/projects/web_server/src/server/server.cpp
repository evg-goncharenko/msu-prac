#include "server.h"

// class SocketAddress:

SocketAddress::SocketAddress() {
    saddr.sin_family = AF_INET; // denotes the addressing family
    saddr.sin_port = htons(PORT); // sets the port number in network byte order
    saddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // sets the IP address
}

SocketAddress::SocketAddress(const char* ip, short port) {
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = inet_addr(ip);
}

SocketAddress::SocketAddress(unsigned int ip, short port) {
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = htonl(ip);
}

// class Socket:

Socket::Socket() {
    // AF_INET - IPv4 protocol (domain - address family);
    // SOCK_STREAM - TCP(reliable, connection oriented) - type of communication
    sd_ = socket(AF_INET, SOCK_STREAM, 0); // the 3rd parameter is a specific protocol
    if (sd_ < 0) {
        std::cout << "Error: Can't create a new socket" << std::endl;
    }
}

// class ServerSocket

void ServerSocket::bind_to(const SocketAddress& ipaddr) { // binding sockets to a specific address:
    if (bind(sd_, ipaddr.get_addr(), ipaddr.get_addr_len()) < 0) { // returns 0 if successful
        std::cout << "Error: Can't bind in ServerSocket" << std::endl;
    }
}

int ServerSocket::accept_to(SocketAddress& client_addr) {
    size_t len = client_addr.get_addr_len();
    int res = accept(sd_, client_addr.get_addr(), (socklen_t *)&len);
    if (res < 0) {
        std::cout << "Error: Can't accept in ServerSocket" << std::endl;
    }
    return res;
}

void ServerSocket::listen_to(int backlog) { // listening state:
    if (listen(sd_, backlog) < 0) {
        std::cout << "Error: Can't listen in ServerSocket" << std::endl;
    }
}

// class ConnectedSocket

void ConnectedSocket::to_write(const std::string& str) {
    if (send(sd_, str.c_str(), str.length(), 0) < 0) { // or write(sd_, buf, buflen)
        std::cout << "Error: Can't write in ConnectedSocket" << std::endl;
    }
}

void ConnectedSocket::to_write(const std::vector<uint8_t>& bytes) {
    if (send(sd_, bytes.data(), bytes.size(), 0) < 0) { // or write(sd_, buf, buflen)
        std::cout << "Error: Can't write in ConnectedSocket" << std::endl;
    }
}

void ConnectedSocket::to_read(std::string& str) { // TODO - rebuild
    int buflen = 4096;
    char buf[buflen];
    if (recv(sd_, buf, buflen, 0) < 0) { // or read(sd_, buf, buflen)
        std::cout << "Error: Can't read in ConnectedSocket" << std::endl;
    }
    str = buf;
}

std::vector<std::string> split_lines(std::string str) {
    std::string delimiter= "\r\n";
    int pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = str.find(delimiter, pos_start)) > 0) {
        token = str.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }
    res.push_back(str.substr(pos_start));
    return res;
}

std::string parse_path(std::string str) {
    std::string res_path = "src/";
    // Analysis, for example, between Get and HTTP:
    for (auto i = 0; i < str.length() - 1; ++i) {
        if (str[i] == ' ') {
            while (str[i + 1] != ' ' && str[i + 1] != '\n') {
                res_path += str[i + 1];
                i++;
            }
            break;
        }
    }
    if (res_path == "src//") { // if it's empty - select the start page
        res_path = "src/index.html";
    }
    return res_path;
}

std::vector<uint8_t> to_vector(int fd) {
    std::vector<uint8_t> v; char c;
    while(read(fd, &c, 1)) v.push_back(c);
    return v;
}

void process_connection(int cd, const SocketAddress& clAddr) {
    ConnectedSocket cs(cd);
    std::string request;
    cs.to_read(request);
    std::vector<std::string> lines = split_lines(request);
    // lines[0] - RequestHeader
    // lines[i] i = 1, ... - HttpHeader
    // lines[lines[lines.size() - 1]] <=> empty line or body
    if (lines.size() > 0) {
        std::cout << lines[0] << std::endl;
    } else {
        std::cout << "Error: lines.size() <= 0 in process_connection()" << std::endl;
    }

    std::string path = parse_path(lines[0]);
    std::cout << "Path: " << path << std::endl;
    
    // Process request:
    int fd = 0;
    
    if ((fd = open(path.c_str(), O_RDONLY)) < 0) {
        std::cout << "HTTP/1.1 404 Not Found" << std::endl;
        cs.to_write("HTTP/1.1 404 Not Found\r");
        if ((fd = open(ERROR_PAGE, O_RDONLY)) < 0) {
            std::cout << "Error: Page 404 is missing" << std::endl;
        }
    } else {
        cs.to_write("HTTP/1.1 200 OK\0");
    }
    std::vector<uint8_t> vect = to_vector(fd);
    std::string str = "\r\nVersion: HTTP/1.1\r\nContent-length: " + std::to_string(vect.size()) + "\r\n\r\n";

    std::cout << "Version: " << "HTTP/1.1" << std::endl;
    std::cout << "Content-length: " << std::to_string(vect.size()) << std::endl;

    cs.to_write(str);
    cs.to_write(vect);
    close(fd);
    cs.shutting_down();
}

void server_loop() {
    SocketAddress server_address(BASE_ADDR, PORT);
    ServerSocket server_socket;
    server_socket.bind_to(server_address); // bind to an address - what port am I on?
    std::cout << "The client was successfully binded" << std::endl;
    server_socket.listen_to(BACKLOG); // listen on a port, and wait for a connection to be established
    for (;;) {
        SocketAddress client_addr;
        int cd = server_socket.accept_to(client_addr);
        process_connection(cd, client_addr); // process cilent-server connection
        std::cout << std::endl;
    }
}