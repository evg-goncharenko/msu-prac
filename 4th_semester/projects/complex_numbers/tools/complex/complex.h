class Complex {
    double re, im; // the real and imaginary parts
    
public:
    Complex();
    
    Complex(double r);
    
    Complex(double r, double i);
    
    Complex(const Complex &c);
    
    ~Complex() {}
    
    Complex& operator = (Complex &c);
    
    Complex& operator += (const Complex &c);
    
    Complex operator + (const Complex &c);
    
    Complex operator - (const Complex &c);
    
    Complex operator * (const Complex &c);
    
    Complex operator / (const Complex &c);
    
    bool operator == (const Complex &c) const;
    
    bool operator != (const Complex &c) const;
    
    // we will specify the friendly operators that we allow access to private data
    friend std::ostream & operator << (std::ostream &, const Complex &);
    friend std::istream & operator >> (std::istream &, Complex &);
    
    double real() const;
    double imag() const;
    
    double abs();
};
