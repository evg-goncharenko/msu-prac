#pragma once

#include <iostream>
#include <stack>
#include <vector>
#include "../identifier/identifier.h"
#include "../lexeme/lexeme.h"
#include "../parser/parser.h"

class Executer {
public:
    void execute(std::vector<Lexeme>& poliz);
};
