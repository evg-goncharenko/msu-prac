#include <algorithm>
#include <iostream>

class BadSegment {};

class BadAddition {
    float a1_, b1_, a2_, b2_;

public:
    BadAddition(float a1, float b1, float a2, float b2) : a1_(a1), b1_(b1), a2_(a2), b2_(b2) {}

    float GetA1() const {
        return a1_;
    }

    float GetB1() const {
        return b1_;
    }

    float GetA2() const {
        return a2_;
    }

    float GetB2() const {
        return b2_;
    }

    ~BadAddition() {}
};

class FloatPair {
    float x_, y_;

public:
    FloatPair() : x_(0), y_(0) {}

    FloatPair(float arg1, float arg2) {
        if (arg1 > arg2) {
            throw BadSegment();
        }
        x_ = arg1;
        y_ = arg2;
    }

    ~FloatPair() {}

    virtual float Measure() const = 0;

    float get_x() const {
        return x_;
    }

    float get_y() const {
        return y_;
    }
};

class Segment : public FloatPair {
public:
    Segment() {}

    Segment(float arg1, float arg2) : FloatPair(arg1, arg2){};

    ~Segment() {}

    Segment operator+(const Segment &s) const {
        if (s.get_x() > this->get_y() || s.get_y() < this->get_x()) {
            throw BadAddition(this->get_x(), this->get_y(), s.get_x(), s.get_y());
        }

        Segment new_seg(std::min(s.get_x(), this->get_x()), std::max(s.get_y(), this->get_y()));
        return new_seg;
    }

    virtual float Measure() const {
        std::cout << "M: " << this->get_y() << std::endl;
        return this->get_y() - this->get_x();
    }
};

int main() {
    try {
        Segment f(1, 2), g(0.5, 5), h(2.5, 6.5);
        std::cout << f.Measure() << std::endl;
        printf("%3.3f, %3.3f, %3.3f\n", (f + g).Measure(), (g + h).Measure(), (f + g + h).Measure());
        printf("%3.3f \n", (f + h).Measure());
    } catch (const BadAddition &bad) {
        printf("Bad addition: [%3.3f; %3.3f] + [%3.3f; %3.3f]\n", bad.GetA1(), bad.GetB1(), bad.GetA2(), bad.GetB2());
    } catch (BadSegment b) {
        printf("Bad segment\n");
    }
    return 0;
}

// result: 4.500, 6.000, 6.000 BadAddition: [1.000; 2.000]+[2.500; 6.500]
