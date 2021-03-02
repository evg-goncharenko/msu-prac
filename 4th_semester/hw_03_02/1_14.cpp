#include <iostream>
using namespace std;

struct smartstr {
    smartstr(int a) { }
private:
    smartstr() { }
};

int main() {
    smartstr s(3);
    smartstr *mass = new smartstr[3];
    return 0;
}
