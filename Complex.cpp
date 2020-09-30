//
// Created by Tsuf Cohen on 2019-01-14.
//

#include "Complex.h"

namespace MtmMath {
    Complex::Complex(double re, double im) : re(re), im(im) {}

    Complex& Complex::operator+=(const Complex& c) {
        re += c.re;
        im += c.im;
        return *this;
    }

    Complex& Complex::operator*=(const Complex& c) {
        Complex tmp;
        tmp.re = ((re*c.re)-(im*c.im));
        tmp.im = ((re*c.im)+(im*c.re));
        *this = tmp;
        return *this;
    }

    Complex Complex::operator-() const {
        return Complex(-re, -im);
    }

    Complex& Complex::operator-=(const Complex& c) {
        return *this += -c;
    }

    bool operator==(const Complex& a, const Complex& b) {
        return a.re==b.re && a.im==b.im;
    }

    bool operator!=(const Complex& a, const Complex& b) {
        return a.re!=b.re || a.im!=b.im;
    }

    ostream& operator<<(ostream& os, const Complex& c) {
        const char* sign = c.im<0 ? "" : "+";
        return os << c.re << sign << c.im << "i";
    }

    istream& operator>>(istream& is, Complex& c) {
        return is >> c.re >> c.im;
    }

    Complex operator+(const Complex& a, const Complex& b) {
        return Complex(a) += b;
    }

    Complex operator*(const Complex& a, const Complex& b) {
        return  Complex(a)*= b;
    }

    Complex operator-(const Complex& a, const Complex& b) {
        return Complex(a) -= b;
    }


}
