#include <iostream>

class Counted {
    static int amount_;
public:
    Counted() { amount_++; }
    Counted(const Counted &c) { amount_++; }
    ~Counted() {}

    static int get() { return amount_; }
};

int Counted::amount_ = 0;

class Watched {
public:
    Watched() {}
    ~Watched() {}

    static int Amount() { return Counted::get(); }
};

int main() {
    Watched w;
    Counted c, c2;
    Counted c3(c2);

    std::cout << "Number of elements: " << Watched::Amount() << std::endl;
    return 0;
}
