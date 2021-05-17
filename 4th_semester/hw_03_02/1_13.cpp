#include <iostream>
using namespace std;

struct mystr {
    mystr() {}
    int a, b;

private:
    mystr(const mystr& s) {
        a = s.a;
        b = s.b;
    }
};

int i = sizeof(mystr);

int f(mystr s) {
    return 0;
}

int main() {
    mystr str;
    f(str);

    return 0;
}
