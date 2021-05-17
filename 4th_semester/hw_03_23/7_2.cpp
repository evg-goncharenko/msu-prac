#include <iostream>

struct B {
    virtual void g() {}
};

struct D : B {
    char y[100];
};

void f9(B &b, D &d, int n) {
    D *pd = (n > 0) ? &d : (D *)&b;

    if (typeid(*pd) == typeid(d)) {
        strcpy(pd->y, "one_variant\n");
        std::cout << "'='" << std::endl;
        std::cout << pd->y;
    }
}

int main() {
    B b;
    D d;
    f9(b, d, 1);
    return 0;
}
