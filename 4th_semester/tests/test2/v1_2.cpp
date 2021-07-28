#include <cctype>
#include <cstdio>
#include <string>

/*
    The parser only needs the type of the token!
    A lexeme is the same as a token - it is a compact and convenient representation of the word
    of the program code for the parser, the most important thing is its type.
    First of all, it is necessary to single out a separate stage of lexical analysis, which collapses
    a bunch of lexemes (a long string into a single number).
    Only its type is needed from the token.
    The token is seen by the lexical and syntactic analyzer.
*/

struct Token {
    enum Type {
        NUL,  // null token when the file is ended
        STR,
        DIG,
        MUL,
        PLUS,
        LPAR,
        RPAR,
        POW
    } type;
    int intval;        // integer token
    std::string sval;  // string token
    Token(Type t = NUL, int iv = 0, std::string str = "") : type(t), intval(iv), sval(str) {}
    void Clear() {  // clears the finished token
        type = NUL;
        intval = 0;
        sval.clear();
    }
};

class Scanner {
protected:   // protected, because function gc() - virtual
    int ch;  // char type in the case of iostream
public:
    Scanner() : ch(' '), curState(S) {}
    virtual ~Scanner() = default;
    Token GetToken() {
        curState = S;
        curToken.Clear();
        while (ch == ' ') {  // skip the spaces
            gc();
        }
        while (curState != FIN) {
            step();
        }
        return curToken;
    }
    Token PeekToken() {
        return curToken;
    }

private:
    Token curToken;
    enum {
        S,
        L,
        D,
        ESC,
        AST,
        FIN
    } curState;
    virtual void gc() = 0;
    void step() {
        switch (curState) {
            case S: {
                if (ch == '"') {
                    curState = L;
                    curToken.type = Token::STR;
                    gc();
                } else if (std::isdigit(ch)) {
                    curState = D;
                    curToken.type = Token::DIG;
                    curToken.intval = ch - '0';
                    gc();
                } else if (ch == '*') {
                    curState = AST;
                    gc();
                } else if (ch == '+') {
                    curState = FIN;
                    curToken.type = Token::PLUS;
                    gc();
                } else if (ch == '(') {
                    curState = FIN;
                    curToken.type = Token::LPAR;
                    gc();
                } else if (ch == ')') {
                    curState = FIN;
                    curToken.type = Token::RPAR;
                    gc();
                } else if (ch == '\n') {
                    curState = FIN;
                    ch = EOF;
                } else if (ch == EOF) {
                    curState = FIN;
                } else {
                    throw "illegal input character";
                }
                break;
            }
            case AST: {
                if (ch == '*') {
                    curToken.type = Token::POW;
                    curState = FIN;
                    gc();
                } else {
                    curToken.type = Token::MUL;
                    curState = FIN;
                }
                break;
            }
            case L: {
                if (ch == '"') {
                    curState = FIN;
                    gc();
                } else if (ch == '\\') {
                    curState = ESC;
                    gc();
                } else {
                    curToken.sval.push_back(ch);
                    gc();
                }
                break;
            }
            case ESC: {
                if (ch == '"' || ch == '\\') {
                    curToken.sval.push_back(ch);
                    gc();
                } else {
                    throw "illegal character after escape";
                }
                curState = L;
                break;
            }
            case D: {
                if (std::isdigit(ch)) {
                    curToken.intval = curToken.intval * 10 + ch - '0';
                    gc();
                } else {
                    curState = FIN;
                }
                break;
            }
            case FIN: {
                throw "unexpected error";
                break;
            }
        }
    }
};

class StreamScanner : public Scanner {
    FILE* fin;

public:
    StreamScanner(FILE* stream = stdin) : fin(stream) {}
    void gc() override {
        ch = fgetc(fin);
    }
};

class StringScanner : public Scanner {
    std::string inp;
    size_t index;

public:
    StringScanner(const char* input) : inp(input), index(0) {}
    void gc() override {
        if (index >= inp.size()) {
            ch = EOF;
        } else {
            ch = inp[index++];
        }
    }
};

class Parser {
    Scanner& s;

public:
    Parser(Scanner& ss) : s(ss) {
        gt();
    }
    void S() {
        T();
        while (ct.type == Token::PLUS) {
            gt();
            T();
        }
    }

private:
    Token ct;
    void gt() {
        ct = s.GetToken();
    }
    void F() {
        if (ct.type == Token::STR) {
            gt();
        } else if (ct.type == Token::LPAR) {
            gt();
            S();
            if (ct.type != Token::RPAR) {
                throw "RParen expected";
            } else {
                gt();
            }
        } else {
            throw "Illegal factor";
        }
    }
    void T() {
        F();
        while (ct.type == Token::MUL) {
            gt();
            if (ct.type != Token::DIG) {
                throw "digit expected";
            }
            gt();
        }
    }
};

int main(int argc, char** argv) {
    int retval = 0;
    Scanner* ps;
    if (argc > 1) {
        ps = new StringScanner(argv[1]);
    } else {
        ps = new StreamScanner(stdin);
    }

    Token token;
    try {
        Parser p(*ps);
        p.S();
        if (ps->PeekToken().type != Token::NUL) {
            throw "extra characters after end of program";
        }
        printf("success\n");
    } catch (const char* msg) {
        fprintf(stderr, "Error: %s\nInterpretation aborted\n", msg);
        retval = 1;
    }
    delete ps;
    return retval;
}