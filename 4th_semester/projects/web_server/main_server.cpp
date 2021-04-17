#include <iostream>

#include "src/server/server.h"

int main() {
    std::cout << CLEAR_SCREEN;
    std::cout << "The server is running on" << " " << RED_COLOR << BASE_ADDR << ":" << PORT << RESET_COLOR << std::endl;
    ServerLoop();
    return 0;
}
