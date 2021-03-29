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

    operator bool() {
        return IsOn();
    }
};

int main() {
    Flag f;
    f.SetOn();
    std::cout << f.IsOn() << std::endl;
    f.SetOff();
    std::cout << f.IsOn() << std::endl;

    f.SetOn();
    if (f) {
        std::cout << "Flag is true" << std::endl;
    }    
    return 0;
}
