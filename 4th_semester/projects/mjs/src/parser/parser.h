#pragma once

#include <iostream>
#include <stack>
#include <vector>
#include "../scanner/scanner.h"

template <class T, class T_EL>

void from_st(T& st, T_EL& i) {
    i = st.top();
    st.pop();
}

class Parser {
    Lexeme curr_lex;
    types_of_lexeme c_type;
    int c_val;
    std::string c_str;
    Scanner scan;
    std::stack<int> st_int;
    std::stack<types_of_lexeme> st_lex;
    std::stack<int> labels_for_con;
    std::stack<int> labels_for_break;
    void S();
    void B();
    void D();
    void E();
    void E1();
    void T();
    void F();
    void WR();
    void GV();
    void FOR_PARAMETERS();
    void dec(types_of_lexeme type, int i);
    void check_id();
    void eq_type(types_of_lexeme&);
    void gl();

public:
    std::vector<Lexeme> poliz;
    Parser() : scan() {}                            // для чтения из перенаправленного ввода
    Parser(std::string program) : scan(program) {}  // для чтения из файла
    void analyze();
};
