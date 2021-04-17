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

void ServerSocket::Bind(const SocketAddress& ipaddr) { // binding sockets to a specific address:
    if (bind(sd_, ipaddr.GetAddr(), ipaddr.GetAddrLen()) < 0) { // returns 0 if successful
        std::cout << "Error: Can't bind in ServerSocket" << std::endl;
    }
}

int ServerSocket::Accept(SocketAddress& client_addr) {
    size_t len = client_addr.GetAddrLen();
    int res = accept(sd_, client_addr.GetAddr(), (socklen_t *)&len);
    if (res < 0) {
        std::cout << "Error: Can't accept in ServerSocket" << std::endl;
    }
    return res;
}

void ServerSocket::Listen(int backlog) { // listening state:
    if (listen(sd_, backlog) < 0) {
        std::cout << "Error: Can't listen in ServerSocket" << std::endl;
    }
}


// class ConnectedSocket

void ConnectedSocket::Write(const std::string& str) {
    const int strlen = str.length();
    char buff[strlen];
    for (auto i = 0; i < strlen; ++i) {
        buff[i] = str[i];
    }
    if (send(sd_, buff, strlen, 0) < 0) { // or write(sd_, buf, buflen)
        std::cout << "Error: Can't write in ConnectedSocket" << std::endl;
    }
}

void ConnectedSocket::WorkFile(int fd) {
    std::string str;
    str += "\r\nContent-length: ";
    char c;
    int len = 0;

    while(read(fd, &c, 1)) len++;
    lseek(fd, 0, 0);
    str += std::to_string(len) + "\r\n\r\n";
    char* buf = (char*) malloc(sizeof(char) * (str.length() + 1));
    strcpy(buf, str.c_str());
    len = strlen(buf);
    send(sd_, buf, len, 0);
    free(buf);

    int buflen = 1024;
    char bufer[buflen];
    while((len = read(fd, bufer, buflen)) > 0){
        send(sd_, bufer, len, 0);
    }
}

void ConnectedSocket::Read(std::string& str) {
    int buflen = 1024;
    char buf[buflen];
    if (recv(sd_, buf, buflen, 0) < 0) { // or read(sd_, buf, buflen)
        std::cout << "Error: Can't read in ConnectedSocket" << std::endl;
    }
    str = buf;
}

std::vector<std::string> split_lines(std::string str) {
    std::string delimiter= "\r\n";
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = str.find(delimiter, pos_start)) != -1) {
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


void ProcessConnection(int cd, const SocketAddress& clAddr) {
    ConnectedSocket cs(cd);
    std::string request;
    cs.Read(request);
    std::vector<std::string> lines = split_lines(request);
    // lines[0] - RequestHeader
    // lines[i] i = 1, ... - HttpHeader
    // lines[lines[lines.size() - 1]] <=> empty line or body
    if (lines.size() > 0) {
        std::cout << lines[0] << std::endl;
    } else {
        std::cout << "Error: lines.size() <= 0 in ProcessConnection()" << std::endl;
    }

    std::string path = parse_path(lines[0]);
    std::cout << "Path: " << path << std::endl;
    
    // Process request:
    int fd = 0;
    
    if ((fd = open(path.c_str(), O_RDONLY)) < 0) {
        std::cout << "HTTP/1.1 404 Not Found" << std::endl;
        cs.Write("HTTP/1.1 404 Not Found");
        if ((fd = open(ERROR_PAGE, O_RDONLY)) < 0) {
            std::cout << "Error: Page 404 is missing" << std::endl;
        }
    } else {
        cs.Write("HTTP/1.1 200 OK");
    }
    
    cs.WorkFile(fd);
    close(fd);
    cs.Shutdown();
}

void ServerLoop() {
    SocketAddress server_address(BASE_ADDR, PORT);
    ServerSocket server_socket;
    server_socket.Bind(server_address); // bind to an address - what port am I on?
    std::cout << "The client was successfully binded" << std::endl;
    server_socket.Listen(BACKLOG); // listen on a port, and wait for a connection to be established
    for (;;) {
        SocketAddress client_addr;
        int cd = server_socket.Accept(client_addr);
        ProcessConnection(cd, client_addr); // process cilent-server connection
    }
}