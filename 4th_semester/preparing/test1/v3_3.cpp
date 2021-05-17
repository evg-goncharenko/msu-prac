#include <iostream>

class M {
    int* obj_;
    int num_;
    static int count;

public:
    M(int n) {
        obj_ = new int;
        *obj_ = n;
        count++;
        num_ = count;
    }

    M() {
        obj_ = new int;
        count++;
        num_ = count;
    }

    ~M() {
        delete obj_;
    }

    int& operator*() const {
        return *obj_;
    }

    int& operator()(M& m) {
        M A(0);
        if (num_ == m.num_) {
            return *obj_;
        }
        return *A;
    }
};

int M::count = 0;

int main() {
    M m(2);
    std::cout << "m: " << m(m) << std::endl;

    M x = 5;
    m(m) = 10;
    std::cout << "m: " << m(m) << std::endl;

    m(m) = 1;
    int tmp = m(m) + m(x) + x(x);
    std::cout << "m: " << m(m) << ", m(x): " << m(x) << ", tmp: " << tmp << std::endl;
    return 0;
}
