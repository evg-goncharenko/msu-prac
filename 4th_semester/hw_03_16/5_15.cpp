#include <iostream>

using namespace std;

struct S {
    S(int a) {
        try {
            if (a > 0) {
                throw * this;
            } else {
                if (a < 0) {
                    throw 0;
                }
            }
        } catch (S &) {
            cout << "SCatch_S&" << endl;
            throw;
        } catch (int) {
            cout << "SCatch_int" << endl;
        }
        cout << "SConstr" << endl;
    }
    S(const S &a) { cout << "Copy" << endl; }
    ~S() { cout << "Destr" << endl; }
};

int main() {
    try {
        S s1(-3), s2(25);
        cout << "Main" << endl;
    } catch (S &) {
        cout << "MainCatch_S&" << endl;
    } catch (...) {
        cout << "MainCatch_..." << endl;
    }
    return 0;
}
/*
    SCatch_int
    SConstr
    Copy
    SCatch_S&
    Destr
    MainCatch_S&
    Destr
*/
