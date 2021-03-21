#include <iostream>

using namespace std;

struct A {
    virtual void z() {}
};

struct B : A {
    int x;
    B(int y = 5) { x = y; }
};

B *f(A *pa) {
    B *p = dynamic_cast<B*>(pa);
    if (p) {
        return p;
    } else {
        throw (string)"bad cast";
    }
}

int main() {
    try {
        B b, *pb = f(&b);
        cout << pb -> x << endl;
        A a;
        pb = f(&a);
        cout << pb -> x << endl;
    } catch (string s) {
        cout << s << endl;
    }
    
    return 0;
}
