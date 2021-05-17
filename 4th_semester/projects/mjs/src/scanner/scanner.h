#pragma once

#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include "../identifier/identifier.h"
#include "../lexeme/lexeme.h"

#define CLEAR_SCREEN "\033[2J\033[1;1H"
#define RESET_COLOR "\033[0m"
#define CYAN_COLOR "\x1b[36m"

extern std::ifstream fp;

class Scanner {
    char c;
    int look(const std::string buf, std::vector<std::string> list);
    bool check();
    void gc();

public:
    static std::vector<std::string> TW, TD;
    Scanner() = default;
    Scanner(const std::string program);
    Lexeme get_lex();
};
