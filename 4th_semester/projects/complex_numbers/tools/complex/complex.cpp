#include <iostream>
#include <cmath>
#include "complex.h"

using namespace std;


Complex::Complex() {
    re = 0;
    im = 0;
}

Complex::Complex(double r) {
    re = r;
    im = 0;
}

Complex::Complex(double r, double i) {
    re = r;
    im = i;
}

Complex::Complex(const Complex &c) { // copy constructor
    re = c.re;
    im = c.im;
}

// assignment operator
Complex& Complex::operator = (Complex &c) {
    re = c.re;
    im = c.im;
    return (*this);
}

// += operator
Complex& Complex::operator+=(const Complex &c) {
    re += c.re;
    im += c.im;
    return *this;
}

// addition operator
Complex Complex::operator + (const Complex &c) {
    return Complex(re + c.re, im + c.im);
}

// subtraction operator
Complex Complex::operator - (const Complex &c) {
    return Complex(re - c.re, im - c.im);
}

// multiplication operator
Complex Complex::operator * (const Complex &c) {
    return Complex(re * c.re - im * c.im, re * c.im + im * c.re);
}

// division operator
Complex Complex::operator / (const Complex &c) {
    Complex temp;
    double r = c.re * c.re + c.im * c.im;
    temp.re = (re * c.re + im * c.im) / r;
    temp.im = (im * c.re - re * c.im) / r;
    return temp;
}

// equality operator
bool Complex::operator == (const Complex &c) const {
    if (im != c.im || re != c.re) {
        return false;
    }
    return true;
}

// inequality operator
bool Complex::operator != (const Complex &c) const {
    if (*this == c) {
        return false;
    }
    return true;
}

double Complex::real() const {
    return re;
}

double Complex::imag() const {
    return im;
}

// modulus of a complex number
double Complex::abs() {
    return sqrt(re * re + im * im);
}

// operator overload <<
ostream& operator << (ostream &out, const Complex &c) {
    out << "(" << c.re << ", " << c.im << ")";
    return out;
}

// operator overload >>
istream& operator >> (istream &in, Complex &c) {
    in >> c.re >> c.im;
    return in;
}
