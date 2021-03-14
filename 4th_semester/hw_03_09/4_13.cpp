#include <iostream>

class B {
public:
    virtual int f();// = 0; - abstruct class
    int g() { return f() * 10; }
    virtual ~B() {}
};

int h(B b) { return b.g() + 2; }

int main() {
    
    return 0;
}
