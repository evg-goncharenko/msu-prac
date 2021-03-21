#include <iostream>

// template <double f> void funcA (double d = f) { /*...*/ } - not int
// template <float f> class A { /*...*/ }; - not int
template <int n> class B { /*...*/ };
template <int n> void funcB (int t = n) { /*...*/ }

template <class Cs> class C { /*...*/ };
template <class Cs> struct D { /*...*/ };
template <class Cs> void funcC (const Cs& ref) { /*...*/ }
class myclass { public: myclass() {} };
// template <myclass c> class E { /*...*/ }; - only class 
struct mystruct { int a, b; };
template <class mystruct> void funcD (mystruct *p = 0) { /*...*/ }
// template <mystruct a> void funcE (mystruct *p = &a) { /*...*/ }
// template <struct mystruct> void funcF (mystruct *p = 0) { /*...*/ }

int main() {
    
    return 0;
}
