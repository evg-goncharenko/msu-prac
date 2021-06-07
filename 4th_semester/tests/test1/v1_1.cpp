#include <iostream>
#include <utility>

using namespace std;

class BadSeq {};

class OrderedSeq;

class BadConcat {
    int l1 = 0, l2 = 0, r1 = 0, r2 = 0;

public:
    BadConcat(const OrderedSeq& s1, const OrderedSeq& s2);
    int GetLeft1() const { return l1; }
    int GetLeft2() const { return l2; }
    int GetRight1() const { return r1; }
    int GetRight2() const { return r2; }
};

class IntSeq {
    int* p_;
    int n_;

public:
    IntSeq(int* pSeq, int n) {
        if (n < 0) {
            throw BadSeq();
        }
        p_ = new int[n_ = n];  // we can immediately assign n_ the value n
        for (int i = 0; i < n; ++i) {
            p_[i] = pSeq[i];
        }
    }

    IntSeq(const IntSeq& other) {
        p_ = new int[n_ = other.n_];
        for (int i = 0; i < n_; ++i) {
            p_[i] = other.p_[i];
        }
    }

    const IntSeq& operator=(const IntSeq& other) {
        if (&other != this) {
            delete[] p_;
            p_ = new int[n_ = other.n_];
            for (int i = 0; i < n_; ++i) {
                p_[i] = other.p_[i];
            }
        }
        return *this;
    }

    IntSeq(IntSeq&& other) noexcept {
        p_ = other.p_;
        n_ = other.n_;
        other.p_ = nullptr;  // we can write: 'other.p_ = new int[0]'
        other.n_ = 0;
    }

    const IntSeq& operator=(IntSeq&& other) {
        if (&other != this) {
            swap(p_, other.p_);
            swap(n_, other.n_);
        }
        return *this;
    }

    int GetLen() const { return n_; }  // must be constant, since Distance() is constant
    const int* GetSeq() const { return p_; }
    virtual int Distance() const = 0;
    virtual ~IntSeq() { delete[] p_; }  // good programming style - virtual destructor
};

class OrderedSeq : public IntSeq {
public:
    OrderedSeq(int* pSeq, int n) : IntSeq(pSeq, n) {
        for (int i = 0; i < n - 1; ++i) {
            if (pSeq[i] >= pSeq[i + 1]) {  // since it stores a strictly ascending sequence of integers
                throw BadSeq();
            }
        }
    }

    int Distance() const override {  // it's useful to write 'const override' because we have a new class
        int max_val = 0;
        int n = GetLen();
        const int* pSeq = GetSeq();
        for (int i = 0; i < n - 1; ++i) {
            int dlt = pSeq[i + 1] - pSeq[i];
            if (dlt > max_val) {
                max_val = dlt;
            }
        }
        return max_val;
    };

    OrderedSeq operator+(const OrderedSeq& s2) {  // no need to pass the value, unnecessary copying
        int n1 = GetLen();
        int n2 = s2.GetLen();
        const int* pS1 = GetSeq();
        const int* pS2 = s2.GetSeq();
        if (n1 != 0 && n2 != 0 && pS1[n1 - 1] >= pS2[0]) {  // it's better to check the correctness first
            throw BadConcat(*this, s2);                     // for simplicity, make the transfer of two parameters, not 4
        }
        int* pNew = new int[n1 + n2];
        for (int i = 0; i < n1; ++i) {
            pNew[i] = pS1[i];
        }
        for (int i = n1; i < n1 + n2; ++i) {
            pNew[i] = pS2[i - n1];
        }
        OrderedSeq s1(pNew, n1 + n2);
        delete[] pNew;
        return s1;
    }
};

BadConcat::BadConcat(const OrderedSeq& s1, const OrderedSeq& s2) {
    int n1 = s1.GetLen();
    int n2 = s2.GetLen();
    const int* pS1 = s1.GetSeq();
    const int* pS2 = s2.GetSeq();
    if (n1 == 0 || n2 == 0) {
        return;
    }
    l1 = pS1[0];
    l2 = pS2[0];
    r1 = pS1[n1 - 1];
    r2 = pS2[n2 - 1];
}

int main() {
    try {
        int a[] = {1, 2, 3};
        int b[] = {7, 9, 10, 13};
        int c[] = {5, 6};
        OrderedSeq sa(a, 3), sb(b, 4), sc(c, 2);
        cout << (sa + sb).Distance() << ", " << (sc + sb).Distance() << ", " << (sa + sc + sb).Distance() << endl;
        cout << (sb + sc).Distance() << endl;
    } catch (const BadConcat& bad) {
        cout << "Bad concatenation " << bad.GetLeft1() << ", " << bad.GetRight1() << " + " << bad.GetLeft2() << ", " << bad.GetRight2() << endl;
    } catch (BadSeq) {
        cout << "Bad sequence\n";
    }
    return 0;
}
