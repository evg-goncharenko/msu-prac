#include <iostream>

class Flag {
    bool condition_; // on - true
public:
    void SetOn() {
        condition_ = true;
    }

    void SetOff() {
        condition_ = false;
    }

    bool IsOn() const {
        return condition_;
    }
};

int main() {
    Flag f;
    f.SetOn();
    std::cout << f.IsOn() << std::endl;
    f.SetOff();
    std::cout << f.IsOn() << std::endl;
    return 0;
}
