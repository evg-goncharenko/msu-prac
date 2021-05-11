#include <iostream>

class Parser {
    enum state {
        S,
        B,
        C,
        F,
        ER
    };
    state CS;
    char c;

public:
    std::string buf;
    int p;

    char gc() {
        if (p >= 0) {
            return (char)buf[p++];
        } else {
            return getchar();
        }
    }

    bool is_allowed() {
        CS = S;
        do {
            c = gc();
            switch (CS) {
                case S: {
                    if (c == 'a') {
                        CS = B;
                    } else {
                        CS = ER;
                    }
                    break;
                }
                case B: {
                    if (c == 'a') {
                        CS = C;
                    } else if (c == 'c') {
                        CS = B;
                    } else if (c == 'b') {
                        CS = S;
                    } else {
                        CS = ER;
                    }
                    break;
                }
                case C: {
                    if (c == 'b') {
                        CS = C;
                    } else if (c == '.') {
                        CS = F;
                    } else {
                        CS = ER;
                    }
                    break;
                }
            }
        } while (CS != F && CS != ER);
        if (CS == ER) {
            std::cout << "NO" << std::endl;
            return 0;
        } else {
            std::cout << "YES" << std::endl;
            return 1;
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
    A.is_allowed();
}
/*

S -> aB
B -> bS
C -> bC
C -> eps
B -> cB
B -> aC
*/