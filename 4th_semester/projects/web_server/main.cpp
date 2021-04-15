#include <iostream>
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

#define SD_BOTH 2
const int BACKLOG = 5;
const std::string DELIMITER = "/r/n";

class SocketAddress {
    struct sockaddr_in saddr;
public:
    SocketAddress();
    SocketAddress(std::string, short port);
    const struct sock_addr* GetAddr() const { return (sock_addr*)&saddr; } // для void Bind()
    // ...
};

class Socket {
protected:
    int sd_;
    explicit Socket(int sd) : sd_(sd) {}
public:
    Socket() {
        sd_ = socket(AF_INET, SOCK_STREAM, 0);
        if (sd_ == -1) {
            throw; // TODO
        }
    }
    void Shutdown() { shutdown(sd_, SD_BOTH); }
    ~Socket() { close(sd_); }
};

class ServerSocket: public Socket{
public:
    void Bind(const SocketAddress& ipaddr) {
        // TODO
    }
    int Accept(SocketAddress& clAddr) {
        // TODO
        return 0;
    }

    void Listen(int backlog) {
        // TODO
    }
};

class ConnectedSocket : public Socket {
public:
    ConnectedSocket() = default;
    explicit ConnectedSocket(int sd) : Socket(sd) {}
    void Write(const std::string& str);
    void Write(const std::vector<uint8_t>& bytes);
    void Read(std::string& str);
    void Read(std::vector<uint8_t>& bytes);
    // ...
};

class ClientSocket : public ConnectedSocket {
public:
    void Connect(const SocketAddress& serverAddr);
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
    SocketAddress saddr("127.0.0.1", 1234);
    s.Connect(saddr);

    //HttpRequest rq;
    // make request ...
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
    //Socket s = Socket(10);
    ServerLoop();
    return 0;
}

