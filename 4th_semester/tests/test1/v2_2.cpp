#include <iostream>

using namespace std;

/*

Example of using an additional structure:

struct Helper {
    int* px;
    Helper(int* p) : px(p) {}
    void operator () (int y) { *px = y; }
};

class Assign {
    int* px;
public:
    Helper operator [] (int& n) { px = &n; return Helper(px); }
    int Get() { return *px; }
}; 

*/

class Assign {
    int* px;

public:
    Assign& operator[](int& n) {
        px = &n;
        return *this;
    }
    int Get() { return *px; }
    void operator()(int y) { *px = y; }
};

int main() {
    Assign a;
    int x = -1, y = 5;
    a[x](y);
    cout << a.Get() << endl;  // print 5
    cout << x << endl;        // print 5
    return 0;
}