#pragma once

#include <iostream>
#include <stack>

#define CLEAR_SCREEN "\033[2J\033[1;1H"
#define RESET_COLOR "\033[0m"
#define CYAN_COLOR "\x1b[36m"

enum types_of_lexeme {
    LEX_NULL,
    LEX_BOOL,
    LEX_BREAK,
    LEX_CONTINUE,
    LEX_DO,
    LEX_ELSE,
    LEX_FALSE,
    LEX_FOR,
    LEX_FUNCTION,
    LEX_GETENV,
    LEX_IF,
    LEX_IN,
    LEX_NAN,
    LEX_NUMBER,
    LEX_NULLPTR,
    LEX_OBJECT,
    LEX_RETURN,
    LEX_STRING,
    LEX_TRUE,
    LEX_TYPEOF,
    LEX_UNDEFINED,
    LEX_VAR,
    LEX_WHILE,
    LEX_WRITE,
    LEX_FIN,
    LEX_SEMICOLON,
    LEX_COMMA,
    LEX_COLON,
    LEX_DOT,
    LEX_LPAREN,
    LEX_RPAREN,
    LEX_LQPAREN,
    LEX_RQPAREN,
    LEX_BEGIN,
    LEX_END,
    LEX_EQ,
    LEX_DEQ,
    LEX_TEQ,
    LEX_LSS,
    LEX_GTR,
    LEX_PLUS,
    LEX_PLUS_EQ,
    LEX_DPLUS,
    LEX_MINUS,
    LEX_MINUS_EQ,
    LEX_DMINUS,
    LEX_TIMES,
    LEX_TIMES_EQ,
    LEX_TIMES_SLASH,
    LEX_SLASH,
    LEX_SLASH_EQ,
    LEX_SLASH_TIMES,
    LEX_DSLASH,
    LEX_PERCENT,
    LEX_PERCENT_EQ,
    LEX_LEQ,
    LEX_NOT,
    LEX_NEQ,
    LEX_NDEQ,
    LEX_GEQ,
    LEX_OR,
    LEX_DPIPE,
    LEX_AND,
    LEX_DAMP,
    LEX_NUM,
    LEX_STR_CONST,
    LEX_ID,
    POLIZ_LABEL,
    POLIZ_ADDRESS,
    POLIZ_GO,
    POLIZ_FGO,
    POLIZ_LEFT_INC,
    POLIZ_RIGHT_INC,
    POLIZ_LEFT_DEC,
    POLIZ_RIGHT_DEC,
    POLIZ_WRITE,
    POLIZ_GETENV
};

class Lexeme {
    types_of_lexeme t_lex;
    int v_lex;
    std::string s_lex;

public:
    Lexeme(types_of_lexeme t = LEX_NULL, int v = 0, std::string s = " ") : t_lex(t), v_lex(v), s_lex(s) {}

    friend std::ostream& operator<<(std::ostream& s, Lexeme l);

    types_of_lexeme get_type() const;
    void put_type(types_of_lexeme t);
    int get_value() const;
    void put_value(int v);
    std::string get_string() const;
    void put_string(std::string s);
};
