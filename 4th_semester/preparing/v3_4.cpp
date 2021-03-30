#include <iostream>

class SumHold {
public:
    int sum_;
    SumHold() : sum_(0) {}
    ~SumHold() {}

    SumHold& operator << (int a) {
        sum_ += a;
        return *this;
    }

    int Get() {
        return sum_;
    }
};

int main() {
    SumHold s1;
    s1 << 1 << 2;
    std::cout << "S1: " << s1.Get() << std::endl;
    SumHold s2;
    s2 << 8 << -2;
    std::cout << "S2: " << s2.Get() << std::endl;
    return 0;
}
