#include <iostream>

class Functor {
public:
    int operator()(const int x, const int y) {
        return x + y;
    }
};

int main() {
    Functor f;
    int x = 3;
    std::cout << f(8, x) << std::endl;
    return 0;
}
