#include <iostream>
#include <math.h>
#include <stdlib.h>

class Complex {
public:
    Complex(); // Default constructor
    ~Complex(); // Default destructor.
    void Set(double new_real, double new_imaginary); // Set data members.
    double Real(); // Return the real part.
    double Imaginary(); // Return the imaginary part.
    Complex operator+(Complex); // Overloaded + operator
    Complex operator*(Complex); // Overloaded * operator
    Complex operator=(Complex); // Overloaded = operator
    int operator==(Complex); // Overloaded == operator

private:
    double real;
    double imaginary;
};

// Complex constructor, initializes to 0 + i0.
Complex::Complex() {
    real = 0;
    imaginary = 0;
}

// Complex destructor.
Complex::~Complex() {}

// Overloaded = operator.
Complex Complex::operator=(Complex c) {
    if (&c != this) {
        real = c.Real();
        imaginary = c.Imaginary();
    }
    return *this;
}

// Overloaded + operator.
Complex Complex::operator+(Complex c) {
    Complex tmp;
    double new_real, new_imaginary;
    new_real = real + c.Real();
    new_imaginary = imaginary + c.Imaginary();
    tmp.Set(new_real, new_imaginary);
    return tmp;
}

// Overloaded * operator.
Complex Complex::operator*(Complex c) {
    Complex tmp;
    double new_real, new_imaginary;
    new_real = real * c.Real() - imaginary * c.Imaginary();
    new_imaginary = real * c.Imaginary() + imaginary * c.Real();
    tmp.Set(new_real, new_imaginary);
    return tmp;
}

// Overloaded == operator. Small error tolerances.
int Complex::operator==(Complex c) {
    // This is to take care of round off errors.
    if (fabs(c.Real() - real) > pow(10, -14)) {
        return 0;
    }
    if (fabs(c.Imaginary() - imaginary) > pow(10, -14)) {
        return 0;
    }
    return 1;
}

// Sets private data members.
void Complex::Set(double new_real, double new_imaginary) {
    real = new_real;
    imaginary = new_imaginary;
}

// Returns the real part of the complex number.
double Complex::Real() {
    return real;
}

// Returns the imaginary part of the complex number.
double Complex::Imaginary() {
    return imaginary;
}
