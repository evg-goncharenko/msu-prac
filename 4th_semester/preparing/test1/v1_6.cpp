#include <iostream>

class A {
public:
    int f;
};

class D {
public:
    A a;
    A* operator -> () {
        return &a;
    }
};

int main() {
    D d1, d2;
    d1 -> f = 5;
    d2 -> f = 3;
    std::cout << "d1 -> f = " << d1 -> f << ", d2 -> f = " << d2 -> f << std::endl;
    return 0;
}
