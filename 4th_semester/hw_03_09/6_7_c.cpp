#include <iostream>

using namespace std;

class A {
    int i;

public:
    A(const int x) { i = x; }
    A(const A &y) { i = y.i; }
    const A f(const A &c) const {
        cout << c.i << endl;
        return *this;
    }
};

const A t1(const A *a) {
    A b = A(3);
    return a->f(b);
}

int main() {
    return 0;
}
