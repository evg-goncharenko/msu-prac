#include <iostream>
using namespace std;

struct A {
    virtual void z() {}
};

struct B : A {
    int x;
    B(int n = 7) {
        x = n;
    }
};

B *f(void *p) {
    B *pb = dynamic_cast<B *>((A *)p);
    if (pb) {
        return pb;
    } else {
        cout << "test" << endl;
        exit(-1);
    }
}

int main() {
    B b, *pb = f(&b);
    cout << pb->x << endl;

    A a;
    pb = f(&a);
    cout << pb->x << endl;
    return 0;
}
