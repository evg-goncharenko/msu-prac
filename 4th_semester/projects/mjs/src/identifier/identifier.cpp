#include "identifier.h"

Identifier::Identifier() {
    declare = false;
    assign = false;
}

Identifier::Identifier(const std::string n) {
    name = n;
    declare = false;
    assign = false;
}

bool Identifier::operator==(const std::string& s) const {
    return name == s;
}

std::string Identifier::get_name() const {
    return name;
}

bool Identifier::get_declare() const {
    return declare;
}

void Identifier::put_declare() {
    declare = true;
}

types_of_lexeme Identifier::get_type() const {
    return type;
}

void Identifier::put_type(types_of_lexeme t) {
    type = t;
}

bool Identifier::get_assign() const {
    return assign;
}

void Identifier::put_assign() {
    assign = true;
}

int Identifier::get_value() const {
    return value;
}

void Identifier::put_value(int v) {
    value = v;
}

std::string Identifier::get_str() const {
    return str;
}

void Identifier::put_str(std::string s) {
    str = s;
}

int put(const std::string& buf) {
    std::vector<Identifier>::iterator k;

    if ((k = find(TID.begin(), TID.end(), buf)) != TID.end())
        return k - TID.begin();
    TID.push_back(Identifier(buf));
    return TID.size() - 1;
}