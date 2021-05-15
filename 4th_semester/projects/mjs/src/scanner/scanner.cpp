#include "scanner.h"

std::ifstream fp;

int Scanner::look(const std::string buf, std::vector<std::string> list) {
    size_t i = 0;
    while (i != list.size()) {
        if (buf == list[i]) {
            return i;
        }
        ++i;
    }
    return 0;
}

bool Scanner::check() {
    return std::cin.eof();
}

void Scanner::gc() {
    std::cin.read(&c, 1);
}

std::vector<std::string> Scanner::TW = {
    "", "Boolean", "break", "continue", "do", "else",
    "false", "for", "function", "getenv", "if", "in",
    "NaN", "Number", "null", "Object", "return", "String",
    "true", "typeof", "undefined", "var", "while", "write"};

std::vector<std::string> Scanner::TD = {
    "@", ";", ",", ":", ".", "(", ")", "[", "]", "{",
    "}", "=", "==", "===", "<", ">", "+", "+=", "++", "-",
    "-=", "--", "*", "*=", "*/", "/", "/=", "/*", "//", "%",
    "%=", "<=", "!", "!=", "!==", ">=", "|", "||", "&", "&&"};

Scanner::Scanner(const std::string program) {
    fp.open(program);
    if (!fp) {
        std::cerr << "File not found\n";
        exit(1);
    }
    std::cin.rdbuf(fp.rdbuf());
}

Lexeme Scanner::get_lex() {
    enum state {
        H,
        IDENT,
        NUMB,
        COM,
        COM2,
        COM3,
        SLSH,
        ALE,
        ALE2,
        PLUS,
        MINUS,
        AMP,
        PIPE,
        QUOTE
    };
    int d, j;
    std::string buf;
    state CS = H;
    do {
        gc();
        switch (CS) {
            case H: {
                if (std::cin.eof()) {
                    return Lexeme(LEX_FIN);
                }
                if (!isspace(c)) {
                    if (isalpha(c)) {
                        buf.push_back(c);
                        CS = IDENT;
                    } else if (isdigit(c)) {
                        d = c - '0';
                        CS = NUMB;
                    } else if (c == '/') {
                        buf.push_back(c);
                        CS = SLSH;
                    } else if (c == '#') {
                        CS = COM3;
                    } else if (c == '!' || c == '=') {
                        buf.push_back(c);
                        CS = ALE;
                    } else if (c == '*' || c == '<' || c == '>' || c == '%') {
                        buf.push_back(c);
                        CS = ALE2;
                    } else if (c == '+') {
                        buf.push_back(c);
                        CS = PLUS;
                    } else if (c == '-') {
                        buf.push_back(c);
                        CS = MINUS;
                    } else if (c == '&') {
                        buf.push_back(c);
                        CS = AMP;
                    } else if (c == '|') {
                        buf.push_back(c);
                        CS = PIPE;
                    } else if (c == '"') {
                        CS = QUOTE;
                    } else if (c == '@') {
                        return Lexeme(LEX_FIN);
                    } else {
                        buf.push_back(c);
                        if ((j = look(buf, TD))) {
                            return Lexeme((types_of_lexeme)(j + (int)LEX_FIN), j);
                        } else {
                            throw c;
                        }
                    }
                }
                break;
            }
            case IDENT: {
                if (isalpha(c) || isdigit(c)) {
                    buf.push_back(c);
                } else {
                    std::cin.unget();
                    if ((j = look(buf, TW))) {
                        return Lexeme((types_of_lexeme)j, j);
                    } else {
                        j = put(buf);
                        return Lexeme((types_of_lexeme)LEX_ID, j);
                    }
                }
                break;
            }
            case NUMB: {
                if (isdigit(c)) {
                    d = d * 10 + (c - '0');
                } else if (isalpha(c)) {
                    throw c;
                } else {
                    std::cin.unget();
                    return Lexeme(LEX_NUM, d);
                }
                break;
            }
            case SLSH: {
                if (c == '*') {
                    buf.pop_back();
                    CS = COM;
                } else if (c == '=') {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lexeme((types_of_lexeme)(j + (int)LEX_FIN), j);
                } else if (c == '/') {
                    buf.pop_back();
                    CS = COM3;
                } else {
                    std::cin.unget();
                    j = look(buf, TD);
                    return Lexeme((types_of_lexeme)(j + (int)LEX_FIN), j);
                }
                break;
            }
            case COM: {
                if (c == '*') {
                    CS = COM2;
                } else if (c == '@' || std::cin.eof()) {
                    CS = H;
                    break;
                }
                break;
            }
            case COM2: {
                if (c == '/') {
                    CS = H;
                } else if (c == '@' || std::cin.eof()) {
                    CS = H;
                    break;
                } else {
                    CS = COM;
                }
                break;
            }
            case COM3: {
                if (c == '\n') {
                    CS = H;
                } else if (c == '@' || std::cin.eof()) {
                    CS = H;
                    break;
                }
                break;
            }
            case ALE: {
                if (c == '=') {
                    buf.push_back(c);
                    CS = ALE2;
                } else {
                    std::cin.unget();
                    j = look(buf, TD);
                    return Lexeme((types_of_lexeme)(j + (int)LEX_FIN), j);
                }
                break;
            }
            case ALE2: {
                if (c == '=') {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lexeme((types_of_lexeme)(j + (int)LEX_FIN), j);
                } else {
                    std::cin.unget();
                    j = look(buf, TD);
                    return Lexeme((types_of_lexeme)(j + (int)LEX_FIN), j);
                }
                break;
            }
            case PLUS: {
                if (c == '=' || c == '+') {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lexeme((types_of_lexeme)(j + (int)LEX_FIN), j);
                } else {
                    std::cin.unget();
                    j = look(buf, TD);
                    return Lexeme((types_of_lexeme)(j + (int)LEX_FIN), j);
                }
                break;
            }
            case MINUS: {
                if (c == '=' || c == '-') {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lexeme((types_of_lexeme)(j + (int)LEX_FIN), j);
                } else {
                    std::cin.unget();
                    j = look(buf, TD);
                    return Lexeme((types_of_lexeme)(j + (int)LEX_FIN), j);
                }
                break;
            }
            case AMP: {
                if (c == '&') {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lexeme((types_of_lexeme)(j + (int)LEX_FIN), j);
                } else {
                    std::cin.unget();
                    j = look(buf, TD);
                    return Lexeme((types_of_lexeme)(j + (int)LEX_FIN), j);
                }
                break;
            }
            case PIPE: {
                if (c == '|') {
                    buf.push_back(c);
                    j = look(buf, TD);
                    return Lexeme((types_of_lexeme)(j + (int)LEX_FIN), j);
                } else {
                    std::cin.unget();
                    j = look(buf, TD);
                    return Lexeme((types_of_lexeme)(j + (int)LEX_FIN), j);
                }
                break;
            }
            case QUOTE: {
                if (c == '"') {
                    std::string res = "";
                    for (size_t i = 0; i < buf.size(); ++i) {
                        res += buf[i];
                    }
                    return Lexeme((types_of_lexeme)LEX_STR_CONST, 0, res);
                } else if (c == '@') {
                    throw c;
                }

                buf.push_back(c);
                break;
            }
        }
    } while (true);
}