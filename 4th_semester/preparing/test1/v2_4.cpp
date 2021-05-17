#include <iostream>

class Help {
public:
    int elem;
    Help(int x) {
        elem = x;
    }

    operator int() const {
        return elem;
    }

    int operator[](int x) const {
        return (elem - 1) * x;
    }
};

class Smart {
public:
    int elem;
    Smart() : elem(0) {}

    Help operator[](int x) const {
        Help tmp(x + 1);
        return tmp;
    }
};

int main() {
    const Smart s;
    int t, x = 3, y = 8;
    t = s[x];
    std::cout << t << std::endl;
    t = s[x][y];
    std::cout << t << std::endl;
    return 0;
}
