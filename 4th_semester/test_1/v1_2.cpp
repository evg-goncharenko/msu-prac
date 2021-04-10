#include <iostream>

using namespace std;

class Cls {
    int static n_;
public:
    Cls& operator [] (int n) { n_ = n; return *this; }
    operator int() { return n_ + 1; }
    void operator >>= (int &x) { x = n_ * 2; }
};

int Cls::n_ = 0;

int main() {
    Cls c;
    int x, N = 5;
    x = c[N];
    cout << x << "\n"; // print 6
    c[N] >>= x;
    cout << x << '\n'; // print 10
    return 0;
}
