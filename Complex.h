#ifndef EX3_COMPLEX_H
#define EX3_COMPLEX_H

#include <iostream>

using std::ostream;
using std::istream;

namespace MtmMath {
    class Complex {
        double re, im;
    public:
        Complex(double re = 0.0, double im = 0.0);
        Complex(const Complex&) = default;
        Complex& operator=(const Complex&) = default;
        Complex& operator+=(const Complex& c);
        Complex& operator*=(const Complex& c);
        Complex operator-() const;
        Complex& operator-=(const Complex& c);
        friend bool operator==(const Complex& a, const Complex& b);
        friend bool operator!=(const Complex& a, const Complex& b);
        friend ostream& operator<<(ostream& os, const Complex& c);
        friend istream& operator>>(istream& is, Complex& c);
    };

    Complex operator+(const Complex& a, const Complex& b);
    Complex operator*(const Complex& a, const Complex& b);
    Complex operator-(const Complex& a, const Complex& b);

}

#endif //EX3_COMPLEX_H
