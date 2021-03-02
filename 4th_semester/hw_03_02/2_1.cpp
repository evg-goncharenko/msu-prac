#include <iostream>
using namespace std;

class Cls {
    int i;
    void operator = (const Cls &s) {
    }
public:
    Cls() { i = 1; }
};

void f(Cls *p, Cls *q) {
    *p = *q;
}

int main() {
    return 0;
}
