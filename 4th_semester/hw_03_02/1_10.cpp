#include <iostream>
using namespace std;

class I {
    int i;
public:
    I() : i(9) { cout << "sun" << endl; }
    I(int a) : i(a) { cout << "venus " << i << endl; }
    I(const I & other) : i(other.i) { cout << "earth " << i << endl; }
    ~I() { cout << "moon" << endl; }
    int Get() { return i; }
    void operator += (const I & op) { i += op.i; }
};

void f(I & x, I y) {
    y += 1000;
    x += y;
}

int main() {
    I i1;
    I i2(20);
    i2 += 400;
    f(i1, i2); // i1 = 9 + 420 + 1000 = 1429; i2 = 420
    cout << i1.Get() << i2.Get() << endl;
    return 0;
}

/*
    sun
    venus 20
    venus 400
    moon
    earth 420
    venus 1000
    moon
    moon
    1429420
    moon
    moon
*/
