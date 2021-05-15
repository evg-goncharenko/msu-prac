#pragma once

#include <stdlib.h>
#include <sys/types.h>
#include <algorithm>
#include <cctype>
#include <iostream>
#include <stack>
#include <vector>
#include "../lexeme/lexeme.h"

class Identifier {
    std::string name;
    bool declare;
    types_of_lexeme type;
    bool assign;
    int value;
    std::string str;

public:
    Identifier();
    Identifier(const std::string n);

    bool operator==(const std::string& s) const;

    std::string get_name() const;
    bool get_declare() const;
    void put_declare();
    types_of_lexeme get_type() const;
    void put_type(types_of_lexeme t);
    bool get_assign() const;
    void put_assign();
    int get_value() const;
    void put_value(int v);
    std::string get_str() const;
    void put_str(std::string s);
};

inline std::vector<Identifier> TID;

int put(const std::string& buf);
