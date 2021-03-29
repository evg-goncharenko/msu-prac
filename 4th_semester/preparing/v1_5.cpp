#include <iostream>

class B;

class A {
public:
    int operator [] (const B &b) {
        return 75;
    }

    int operator () () {
        return 50;
    }

    int operator() (const B &b) {
        return 100;
    }

    int operator () (const B &b1, const B &b2) {
        return 150;
    }

    int operator -- () {
        return 500;   
    }

    int operator -- (int b) {
        return 501;
    }

    int operator () (const A &a1, const A &a2, const A &a3) {
        throw a1;
    }
};

class B {
public:
    int operator [] (const A &a) {
        return 75;
    }

    int operator () (const A &a) {
        return 200;
    }
};

int main() {
    A a;
    B b;
    std::cout << "a[b] = " << a[b] << ", b[a] = " << b[a] << std::endl; // 75
    std::cout << "a() = " << a() << std::endl;  // 50
    std::cout << "a(b) = " << a(b) << std::endl;  // 100
    std::cout << "a(b, b) = " << a(b, b) << std::endl;  // 150
    std::cout << "b(a) = " << b(a) << std::endl;  // 200 
    // a(a, a, a) - successful compilation, calculate - exception A
    std::cout << "--a = " << --a << std::endl;  // 500 
    std::cout << "a-- = " << a-- << std::endl;  // 501 
    return 0;
}
