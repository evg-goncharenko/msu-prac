#include "lexeme.h"
#include "../scanner/scanner.h"

std::ostream& operator<<(std::ostream& s, Lexeme l) {
    std::string t;
    if (l.t_lex <= 23) {
        t = Scanner::TW[l.t_lex];
    } else if (l.t_lex >= 24 && l.t_lex <= 63) {
        t = Scanner::TD[l.t_lex - 24];
    } else if (l.t_lex == 64) {
        t = "NUMB";
    } else if (l.t_lex == 65) {
        t = "STRING CONST";
        s << '(' << t << ',' << l.s_lex << ");" << std::endl;
        return s;
    } else if (l.t_lex == 66) {
        t = TID[l.v_lex].get_name();
    } else if (l.t_lex == 67) {
        t = "Label";
    } else if (l.t_lex == 68) {
        t = "&";
    } else if (l.t_lex == 69) {
        t = "!";
    } else if (l.t_lex == 70) {
        t = "!F";
    } else if (l.t_lex == 71) {
        t = "+#";
    } else if (l.t_lex == 72) {
        t = "#+";
    } else if (l.t_lex == 73) {
        t = "-#";
    } else if (l.t_lex == 74) {
        t = "#-";
    } else if (l.t_lex == 75) {
        t = "wr";
    } else if (l.t_lex == 76) {
        t = "gv";
    } else {
        throw l;
    }
    s << '(' << t << ',' << l.v_lex << ");" << std::endl;
    return s;
}

types_of_lexeme Lexeme::get_type() const {
    return t_lex;
}

void Lexeme::put_type(types_of_lexeme t) {
    t_lex = t;
}

int Lexeme::get_value() const {
    return v_lex;
}

void Lexeme::put_value(int v) {
    v_lex = v;
}

std::string Lexeme::get_string() const {
    return s_lex;
}

void Lexeme::put_string(std::string s) {
    s_lex = s;
}