#include <iostream>

using namespace std;

struct B {
    virtual void f(int n) {
        cout << "f(int) from B" << endl;
    }
    static int i;
};

struct D: B {
    virtual void f(char n) {
        cout << "f(char) from D" << endl;
    }
};

int B::i = 1;

int main() {
    D d;
    B b1, b2, *pb = &d;
    pb -> f('a'); // f(int) from B
    b1.i += 2;
    b2.i += 3;
    d.i += 4;
    cout << b1.i << ' ' << b2.i << ' ' << B::i << endl; // 10 10 10
    return 0;
}
