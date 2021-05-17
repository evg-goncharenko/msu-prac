#include <iostream>

class Assign {
public:
    Assign *operator->() {
        return this;
    }
    template <class T>
    void f(T &a, T &b) {
        a = b;
    }
};

int main() {
    Assign a;
    bool x = true, y = false;
    a->f(x, y);
    std::cout << "x: " << x << ", y: " << y << std::endl;
    return 0;
}
