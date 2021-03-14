#include <iostream>

struct S {
    virtual void f();// const = 0; - abstruct class
    virtual ~S() {}
};

struct A {
    S s;
    int i;
};

int main() {
    
    return 0;
}
