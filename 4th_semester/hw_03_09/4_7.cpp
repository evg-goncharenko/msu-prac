#include <iostream>

using namespace std;

class T {
public:
    virtual void f(int x) {
        h();
        cout << "T::f," << x << endl;
    }
    void g() {
        h();
        cout << "T::g" << endl;
    }
    virtual void h() {
        cout << "T::h" << endl;
    }
};
    
class U: virtual public T {
public:
    void f(int y) {
        h(y); cout << "U::f," << y << endl;
    }
    virtual void g() {
        h(0); cout << "U::g" << endl;
    }
    void h (int k) { cout << "U::h," << k << endl; }
};

int main() {
    U u; T * p = & u;
    p -> f(1); // U::h,1 U::f,1
    p -> g(); // T::h T::g
    p -> h(); // T::h
    //p -> h(2);
    
    return 0;
}
