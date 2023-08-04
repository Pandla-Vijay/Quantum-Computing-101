#include <iostream>
#include <math.h>
#include "qureg.cpp"

#define PI 3.14159265359

// This function takes an integer input and returns 1 if it is a prime
// number, and 0 otherwise.
int TestPrime(int n) {
    int i;
    for (i = 2; i <= floor(sqrt(n)); i++) {
        if (n % i == 0) {
            return (0);
        }
    }
    return (1);
}

// This function takes an integer input and returns 1 if it is equal to a
// prime number raised to an integer power, and 0 otherwise.
int TestPrimePower(int n) {
    int i, j;
    j = 0;
    i = 2;
    while ((i <= floor(pow(n, .5))) && (j == 0)) {
        if ((n % i) == 0) {
            j = i;
        }
        i++;
    }
    for (int i = 2; i <= (floor(log(n) / log(j)) + 1); i++) {
        if (pow(j, i) == n) {
            return (1);
        }
    }
    return (0);
}

// This function computes the greatest common denominator of two integers.
// Since the modulus of a number mod 0 is not defined, we return a -1 as
// an error code if we ever would try to take the modulus of something and
// zero.
int GCD(int a, int b) {
    int d;
    if (b != 0) {
        while (a % b != 0) {
            d = a % b;
            a = b;
            b = d;
        }
    } else {
        return -1;
    }
    return (b);
}

// This function takes an integer argument, and returns the size in bits
// needed to represent that integer.
int RegSize(int a) {
    int size = 0;
    while (a != 0) {
        a = a >> 1;
        size++;
    }
    return (size);
}

// q is the power of two such that n^2 <= q < 2n^2.
int GetQ(int n) {
    int power = 8; // 265 is the smallest q ever is.
    while (pow(2, power) < pow(n, 2)) {
        power = power + 1;
    }
    return ((int)pow(2, power));
}

// This function takes three integers, x, a, and n, and returns x^a mod n.
// This algorithm is known as the "Russian peasant method," I believe.
int modexp(int x, int a, int n) {
    int value = 1;
    int tmp;
    tmp = x % n;
    while (a > 0) {
        if (a & 1) {
            value = (value * tmp) % n;
        }
        tmp = tmp * tmp % n;
        a = a >> 1;
    }
    return value;
}

// This function finds the denominator q of the best rational
// denominator q for approximating p / q for c with q < qmax.
int denominator(double c, int qmax) {
    double y = c;
    double z;
    int q0 = 0;
    int q1 = 1;
    int q2 = 0;
    while (1) {
        z = y - floor(y);
        if (z < 0.5 / pow(qmax, 2)) {
            return (q1);
        }
        if (z != 0) {
            // Can’t divide by 0.
            y = 1 / z;
        } else {
            // Warning this is broken if q1 == 0, but that should never happen.
            return (q1);
        }
        q2 = (int)floor(y) * q1 + q0;
        if (q2 >= qmax) {
            return (q1);
        }
        q0 = q1;
        q1 = q2;
    }
}

// This function takes two integer arguments and returns the greater of
// the two.
int max(int a, int b) {
    if (a > b) {
        return (a);
    }
    return (b);
}

// This function computes the discrete Fourier transformation on a register’s
// 0 -> q - 1 entries.
void DFT(QuReg* reg, int q) {
    // The Fourier transform maps functions in the time domain to
    // functions in the frequency domain. Frequency is 1/period, thus
    // this Fourier transform will take our periodic register, and peak it
    // at multiples of the inverse period. Our Fourier transformation on
    // the state a takes it to the state: q^(-.5) * Sum[c = 0 -> c = q - 1,
    // c * e^(2*Pi*i*a*c / q)]. Remember, e^ix = cos x + i*sin x.
    Complex* init = new Complex[q];
    Complex tmpcomp;
    tmpcomp.Set(0, 0);

    // Here we do things that a real quantum computer couldn’t do,
    // such as look at individual values without collapsing state.
    // The good news is that in a real quantum computer, you could build a gate
    // which would do this all in one step.
    int count = 0;
    double tmpreal = 0;
    double tmpimag = 0;
    double tmpprob = 0;
    for (int a = 0; a < q; a++) {
        // This if statement helps prevent previous round-off errors from
        // propagating further.
        if ((pow(reg->GetProb(a).Real(), 2) + pow(reg->GetProb(a).Imaginary(), 2)) > pow(10, -14)) {
            for (int c = 0; c < q; c++) {
                tmpcomp.Set(pow(q, -.5) * cos(2 * PI * a * c / q), pow(q, -.5) * sin(2 * PI * a * c / q));
                init[c] = init[c] + (reg->GetProb(a) * tmpcomp);
            }
        }
        count++;
        if (count == 100) {
            std::cout << "Making progress in Fourier transform, " << 100 * ((double)a / (double)(q - 1)) << "% done!" << std::endl << std::flush;
            count = 0;
        }
    }
    reg->SetState(init);
    reg->Norm();
    delete[] init;
}
