#ifndef SERVER_H
#define SERVER_H

#include <cstddef>
#include <iostream>
#include <sys/fcntl.h>
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

const int FULL_SHUTDOWN =  2; // in both directions
const int BACKLOG = 5;

const int PORT = 8080;
#define BASE_ADDR "127.0.0.1" 
#define ERROR_PAGE "src/404.html"

#define CLEAR_SCREEN "\033[2J\033[1;1H"
#define RESET_COLOR "\033[0m"
#define CYAN_COLOR "\x1b[36m"

class SocketAddress {
    struct sockaddr_in saddr; // can store a pair: IP address + port
public:
    SocketAddress();
    SocketAddress(const char* ip, short port);
    SocketAddress(unsigned int ip, short port);
    // get_addr() for the bind_to() function - returns a pointer to the structure containing the address
    struct sockaddr* get_addr() const { return (sockaddr*)&saddr; }
    int get_addr_len() const { return sizeof(saddr); }
};

class Socket {
protected:
    int sd_;
    explicit Socket(int sd) : sd_(sd) {}
public:
    Socket();
    void shutting_down() { shutdown(sd_, FULL_SHUTDOWN); } // shutting down the socket 
    ~Socket() { close(sd_); } // closing the socket
};

class ServerSocket: public Socket{
public:
    void bind_to(const SocketAddress& ipaddr);
    int accept_to(SocketAddress& client_addr);
    void listen_to(int backlog);
};

class ConnectedSocket : public Socket {
public:
    ConnectedSocket() = default;
    explicit ConnectedSocket(int sd) : Socket(sd) {} // TODO
    void to_write(const std::string& str);
    void to_write(const std::vector<uint8_t>& bytes);
    void to_read(std::string& str);
    void to_read(std::vector<uint8_t>& bytes) {} // TODO
};

std::vector<std::string> split_lines(std::string str);
std::string parse_path(std::string str);
void process_connection(int cd, const SocketAddress& clAddr);
void server_loop();

#endif // SERVER_H