#include <iostream>

class Assign {
    int val_;
public:
    Assign() : val_(0) {}
    ~Assign() {}

    Assign* operator -> () {
        return this;
    }

    void f(int &a, int &b) {
        a = b;
        val_ = b;
    }

    int Get() {
        return val_;
    }
};

int main() {
    Assign a;
    int x = 1, y = 2;
    a -> f(x, y);
    std::cout << "x: "<< x << ", y: " << y << std::endl;
    std::cout << ".Get(): " << a.Get() << std::endl;
    return 0;
}
