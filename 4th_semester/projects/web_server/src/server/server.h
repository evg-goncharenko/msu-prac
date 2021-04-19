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
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define FULL_SHUTDOWN   2 // in both directions
#define BACKLOG         5

#define PORT            8080
#define BASE_ADDR       "127.0.0.1" 
#define ERROR_PAGE      "src/404.html"

#define CLEAR_SCREEN    "\033[2J\033[1;1H"
#define RESET_COLOR     "\033[0m"
#define CYAN_COLOR      "\x1b[36m"

#define CONTENT_TYPE    "CONTENT_TYPE=text/plain"
#define REMOTE_ADDR     "REMOTE_ADDR=127.0.0.5"
#define REMOTE_PORT     "REMOTE_PORT=8845"
#define QUERY_STRING    "QUERY_STRING="
#define SERVER_ADDR     "SERVER_ADDR=127.0.0.1"
#define SERVER_NAME     "www.primarysite.com"
#define SERVER_PORT     "SERVER_PORT=8080"
#define SERVER_PROTOCOL "SERVER_PROTOCOL=HTTP/1.0"
#define SCRIPT_NAME     "SCRIPT_NAME="

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
    explicit ConnectedSocket(int sd) : Socket(sd) {}
    void to_write(const std::string& str);
    void to_write(const std::vector<uint8_t>& bytes);
    void to_read(std::string& str);
    void to_read(std::vector<uint8_t>& bytes) {}
};

std::vector<std::string> split_lines(std::string str); // splitting request into lines
std::string parse_path(std::string str);               // extracting the path to an object
std::vector<uint8_t> to_vector(int fd);                // reading from a file descriptor to a vector
std::string get_cgi_file_name(std::string path);
std::string get_cgi_query(std::string path);
char** create_array(std::vector<std::string> &v);      // create array char** from vector<string>
void check_error(ConnectedSocket cs);                  // errors handling
bool is_cgi_connection(std::string str);
void cgi_connection(std::string path, int cd, const SocketAddress& client_addr, ConnectedSocket cs);
void default_connection(std::string path, ConnectedSocket cs);
void process_connection(int cd, const SocketAddress& clAddr);
void server_loop();

#endif // SERVER_H