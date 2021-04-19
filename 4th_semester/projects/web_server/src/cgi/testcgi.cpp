#include <iostream> 
#include <stdlib.h>

int main () {
    std::cout << "<html><body>\n";
    std::cout << "Welcome to CGI test program\n";
    std::cout << "Here is what I've got\n"; 
    std::cout << "SERVER_ADDR: "      << getenv("SERVER_ADDR") << std::endl;
    std::cout << "SERVER_PORT: "      << getenv("SERVER_PORT") << std::endl; 
    std::cout << "SERVER_PROTOCOL: "  << getenv("SERVER_PROTOCOL") << std::endl; 
    std::cout << "CONTENT_TYPE: "     << getenv("CONTENT_TYPE") << std::endl;
    std::cout << "QUERY_STRING: "     << getenv("QUERY_STRING") << std::endl;
    std::cout << "SCRIPT_NAME: "      << getenv("SCRIPT_NAME") << std::endl;
    
    std::cout << "</body></html>\n";
    return 0; 
}
