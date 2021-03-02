#include <iostream>
using namespace std;

class A {
    int c;
public:
    A(int a) { c = a; }
    int operator *= (int b) {
        c = c * b;
        return c;
    }
    int get() {
        return c;
    }
};

int main() {
    A a1(5), a2 = 3;
    a1 *= 10;
    a2 *= a1 *= 2;
    cout << a1.get() << " " << a2.get() << endl;
    return 0;
}
