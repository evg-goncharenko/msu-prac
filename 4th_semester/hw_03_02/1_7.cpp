#include <iostream>
using namespace std;

class A {
    int c;
public:
    A(int a = 7) { c = a; }
    
    int operator *= (int b) {
        c = c * b;
        return c;
    }
    int get() {
        return c;
    }
};

int main() {
    A a1(5), a2 = 4, a3;
    a2 *= a1 *= 3;
    cout << a1.get() << " " << a2.get() << " " << a3.get() << endl;
    return 0;
}
