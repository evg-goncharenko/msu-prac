#include <iostream>
class SumHold {
    static int all_sum_;

public:
    int sum_;
    SumHold() : sum_(0) {}
    ~SumHold() {}

    SumHold& operator<<(int a) {
        all_sum_ += a;
        sum_ += a;
        return *this;
    }

    int Get() {
        return sum_;
    }

    int GetAll() {
        return all_sum_;
    }
};

int SumHold::all_sum_ = 0;

int main() {
    SumHold s1;
    s1 << 1 << 2;
    std::cout << "S1: " << s1.Get() << std::endl;
    SumHold s2;
    s2 << 8 << -2;
    std::cout << "S2: " << s2.Get() << std::endl;
    std::cout << "All Sum: " << s2.GetAll() << std::endl;
    return 0;
}
