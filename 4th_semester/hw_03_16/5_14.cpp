#include <iostream>

using namespace std;

struct S {
    S(int a) {
        try {
            if (a > 0) {
                throw *this;
            } else {
                if (a < 0) {
                    throw 0;
                }
            }
        }
        catch (S & a) {
            cout << "SCatch_S&" << endl;
        }
        catch (int) { throw; }
        cout << "SConstr" << endl;
    }
    S(const S & a) { cout << "Copy" << endl; }
    ~S() { cout << "Destr" << endl; }
};

int main() {
    try { S s1(0), s2(5);
        cout << "Main" << endl;
    }
    catch (S &) { cout << "MainCatch_S&" << endl; }
    catch ( ... ) { cout << "MainCatch_..." << endl; }
    return 0;
}

/*
    SConstr
    Copy
    SCatch_S&
    Destr
    SConstr
    Main
    Destr
    Destr
*/
