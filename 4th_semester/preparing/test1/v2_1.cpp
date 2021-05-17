#include <iostream>

class A {
    static int numb_;

public:
    static int f() {
        return numb_++;
    }
};

int A::numb_ = 0;

int main() {
    A a;
    std::cout << a.f() << std::endl;
    std::cout << a.f() << std::endl;
    std::cout << A::f() << std::endl;
    return 0;
}
