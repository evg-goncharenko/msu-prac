#include <iostream>
#include <sys/_endian.h>
#include <sys/_types/_socklen_t.h>
#include <vector>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <netdb.h>
#include <unistd.h>
#include <string>
#include <sys/stat.h>
#include <fcntl.h>

const int SD_BOTH =  2;
const int BACKLOG = 5;
const std::string DELIMITER = "/r/n";

class SocketAddress {
    struct sockaddr_in saddr;
public:
    SocketAddress() {
        saddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(8080);
    }
    SocketAddress(const char* ip, short port) {
        saddr.sin_addr.s_addr = inet_addr(ip);
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(port);
    }
    SocketAddress(unsigned int ip, short port) {
        saddr.sin_addr.s_addr = htonl(ip);
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(port);
    }
    int GetSize() const { return sizeof(saddr); }
    struct sockaddr* GetAddr() const { return (sockaddr*)&saddr; } // for void Bind()
    // ...
};

class Socket {
protected:
    int sd_;
    explicit Socket(int sd) : sd_(sd) {}
public:
    Socket() {
        sd_ = socket(AF_INET, SOCK_STREAM, 0); // AF_INET - IPv4 protocol; SOCK_STREAM - TCP(reliable, connection oriented)
        if (sd_ == -1) {
            throw; // TODO
        }
    }
    
    void Shutdown() { shutdown(sd_, SD_BOTH); } // shutdown to end read/write; 

    ~Socket() { close(sd_); }
};

class ServerSocket: public Socket{
public:
    void Bind(const SocketAddress& ipaddr) {
        if (bind(sd_, ipaddr.GetAddr(), ipaddr.GetSize()) < 0) {
            throw;
            // TODO
        }
    }

    int Accept(SocketAddress& client_addr) {
        size_t len = client_addr.GetSize();
        int res = accept(sd_, client_addr.GetAddr(), (socklen_t *)&len);
        if (res < 0) {
            throw;
            // TODO
        }
        return res;
    }

    void Listen(int backlog) {
        if (listen(sd_, backlog) < 0) {
            throw;
            // TODO
        }
    }
};

class ConnectedSocket : public Socket {
public:
    ConnectedSocket() = default;
    explicit ConnectedSocket(int sd) : Socket(sd) {} // TODO
    void Write(const std::string& str) {
        //write(sd_, const void *__buf, size_t __nbyte)
    } // TODO
    void Write(const std::vector<uint8_t>& bytes) {} // TODO
    void Read(std::string& str) {
        const int buflen = 1024;
        char buf[buflen];
        if (recv(sd_, buf, buflen, 0) < 0) {
            throw;
            // TODO
        }
        str = buf;
    }
    // TODO  
    void Read(std::vector<uint8_t>& bytes) {} // TODO
    // ...
};

class ClientSocket : public ConnectedSocket {
public:
    void Connect(const SocketAddress& serverAddr) {}
};

class HttpHeader {
    std::string name;
    std::string value;
public:
    HttpHeader(const std::string& n, const std::string& v) : name(n), value(v) {} // server <=> ResponceHeader
    static HttpHeader* ParseHeader(const std::string& line);
    explicit HttpHeader(const std::string& line); // server <=> RequestHeader
};

std::vector<std::string> split_lines(std::string str) {
    size_t pos_start = 0, pos_end, delim_len = DELIMITER.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = str.find(DELIMITER, pos_start)) != std::string::npos) {
        token = str.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }
    res.push_back(str.substr(pos_start));
    return res;
}

void ProcessConnection(int cd, const SocketAddress& clAddr) {
    ConnectedSocket cs(cd);
    std::string request;
    cs.Read(request);
    std::vector<std::string> lines = split_lines(request);
    if (lines.size() > 0) {
        std::cout << lines[0] << std::endl;
    } else {
        std::cout << "size <= 0" << std::endl;
    }
    
    // lines[0] - RequestHeader
    // lines[i] i = 1, ... - HttpHeader
    // lines[lines[lines.size() - 1]] <=> empty line

    // process request
    // make response
    cs.Shutdown();
}

void ServerLoop() {
    SocketAddress server_address("127.0.0.1", 8080);
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

void ClientConnection() {
    ClientSocket s;
    SocketAddress saddr("127.0.0.1", 8080);
    s.Connect(saddr);

    //HttpRequest rq;
    //make request ...
    //s.Write(rq.ToString());
    std::string strResponce;
    s.Read(strResponce);
    //HttpResponce resp;
    //std::vector<std::string> lines = SplitLines(strResponce);
    // parce responce ...
    s.Shutdown();
}

int main() {
    std::cout << "Start program" << std::endl;
    ServerLoop();
    return 0;
}

