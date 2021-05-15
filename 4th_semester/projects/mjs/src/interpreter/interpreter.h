#pragma once

#include <iostream>
#include <stack>
#include "../executer/executer.h"
#include "../parser/parser.h"

class Interpreter {
    Parser parse;
    Executer executer;

public:
    Interpreter() : parse(){};                            // to read from redirected input
    Interpreter(std::string program) : parse(program){};  // to read from a file
    void interpretation();                                // code analysis and execution
};
