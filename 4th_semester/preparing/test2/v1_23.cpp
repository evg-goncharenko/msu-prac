#include <iostream>

class Parser {
public:
    char c;
    std::string buf;
    int p;
    int sum;

    void gc() {
        if (p >= 0) {
            c = (char)buf[p++];
        } else {
            c = getchar();
        }
    }
    int S() {
        int sum = 0;
        char d;
        sum += F();
        std::cout << sum << "\t";
        while ((c == '+') || (c == '-')) {
            d = c;
            A();
            if (d == '+') {
                sum += F();
            } else {
                sum -= F();
            }
            std::cout << sum << "\t";
        }
        return sum;
    }
    int F() {
        int mult;
        if (c == '-') {
            gc();
            mult = -F();
        } else if (c == '(') {
            gc();
            mult = S();
            if (c == ')') {
                gc();
            } else {
                throw(c);
            }
        } else {
            mult = I();
        }
        return mult;
    }
    void A() {
        if ((c == '+') || (c == '-')) {
            gc();
        } else {
            throw(c);
        }
    }
    int I() {
        int mult;
        mult = c - '0';
        D();
        while (isdigit(c)) {
            mult = mult * 10 + c - '0';
            D();
        }
        return (mult);
    }
    void D() {
        if (isdigit(c)) {
            gc();
        } else {
            throw(c);
        }
    }
};

int main(int argc, char** argv) {
    Parser A;
    if (argc == 2) {
        A.p = 0;
        A.buf = (std::string)argv[1];
    } else {
        A.p = -1;
    }

    try {
        A.gc();
        A.S();
        if ((A.c != '.') && (A.c != '\n') && (A.c != '\0')) {
            throw(A.c);
        }
        std::cout << "YES" << std::endl;
    } catch (char c) {
        std::cout << "NO " << c << std::endl;
    }
}

/*
S -> F{AF}
F -> -F | I | (S)
A -> + | -
I -> D {D}
D -> 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
*/