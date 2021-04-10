#include <iostream>

using namespace std;

class BadRectangle {};

class BadAdd {
    int x1, y1, x2, y2;
public:
    BadAdd(int X1, int Y1, int X2, int Y2) : x1(X1), y1(Y1), x2(X2), y2(Y2) {}
    int GetX1() const { return x1; }
    int GetX2() const { return x2; }
    int GetY1() const { return y1; }
    int GetY2() const { return y2; }
};

class IntPair {
    int x_, y_;
public:
    virtual int Measure() const = 0;
    IntPair(int x, int y) : x_(x), y_(y) {}
    int GetX() const { return x_; }
    int GetY() const { return y_; }
};

class IntRectangle : public IntPair {
public:
    IntRectangle (int x = 0, int y = 0) : IntPair(x, y) {
        if (x < 0 || y < 0) {
            throw BadRectangle();
        }
    }

    virtual int Measure() const { return GetX() * GetY(); }

};

IntRectangle operator + (const IntRectangle& r1, const IntRectangle& r2) {
    if (r1.GetX() == r2.GetX()) {
        return IntRectangle(r1.GetX(), r1.GetY() + r2.GetY());
    } else if (r1.GetY() == r2.GetY()) {
        return IntRectangle(r1.GetX() + r2.GetX(), r1.GetY());
    } else {
        throw BadAdd(r1.GetX(), r1.GetY(), r2.GetX(), r2.GetY());
    }
}

int main() { 
    try {
        IntRectangle a(5, 4), b(7, 4), c(2, 1), d(2, 3);
        IntRectangle p, q;
        p = a + b;
        q = c + d;
        cout << (p+q).Measure() << endl;
        cout << (p+d).Measure() << endl; 
    }
    catch (const BadAdd &bad) {
        cout << "Bad add: (" << bad.GetX1() << " x " << bad.GetY1() << ") + (" << bad.GetX2() << " x " << bad.GetY2() << ")\n";  
    }
    catch (BadRectangle b) { 
        cout << "Bad rectangle\n"; 
    }       
    return 0; 
}
