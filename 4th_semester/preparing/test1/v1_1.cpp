#include <iostream>

void set_min(int &a, int &b) {
    if (a < b) {
        b = a;
    } else {
        a = b;
    }
}

int main() {
    int a = 1, b = 2;
    set_min(a, b);
    std::cout << a << " " << b << std::endl;
    return 0;
}
