#include <iostream>

using namespace std;

class A {
    int i;
public:
    A(const int x) { i = x; }
    A(const A & y) { i = y.i; }
    const A f(const A & z) const {
        cout << endl;
        return *this;
    }
};

const A t1() {
    const A a = 5;
    return a.f(a);
}

int main() {
    
    return 0;
}
