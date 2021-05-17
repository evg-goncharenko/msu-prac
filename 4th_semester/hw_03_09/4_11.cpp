#include <iostream>

struct A {
    int i;
    virtual void f();  // = 0 - abstruct class
    virtual ~A() {}
};

int g(A a) { return a.i * 5; }

int main() {
    return 0;
}
