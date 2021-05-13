#include <iostream>

class PseudoArray {
    int body_;
public:
    PseudoArray() : body_(0) {}
    ~PseudoArray() {}

    int &operator [] (int index) {
        body_ = 0;
        return body_;
    }

    const int &operator [] (int index) const {
        return body_;
    }

    PseudoArray &operator = (const PseudoArray &r) {
        return *this;
    }
};

int main() {
    PseudoArray arr;
    arr[100] = 6;
    std::cout << arr[0] << " " << arr[100] << std::endl;
    return 0;
}
