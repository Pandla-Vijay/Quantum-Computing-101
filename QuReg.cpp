#include <iostream>
#include<math.h>
#include<time.h>

class QuReg {
public:
    // Default constructor. Size is the size in bits of our register.
    // In our implementation of Shor’s algorithm we will need size bits
    // to represent our value for "q" which is a number we have chosen
    // with small prime factors which is between 2n^2 and 3n^2 inclusive
    // where n is the number we are trying to factor. We envision our the
    // description of our register of size "S" as 2^S complex number,
    // representing the probability of finding the register on one of or
    // 2^S base states. Thus we use an array of size 2^S, of Complex
    // numbers. Thus if the size of our register is 3 bits array[7] is
    // the probability amplitude of the state |1,1,1>, and array[7] *
    // Complex Conjugate(array[7]) = probability of choosing that state.
    // We use normalised state vectors thought the simulation, thus the
    // sum of all possible states times their complex conjugates is = 1.
    QuReg(int size);
    QuReg(); // Default Constructor
    QuReg(const QuReg &); // Copy constructor
    ~QuReg(); // Default destructor.

    int DecMeasure(); // Measures our quantum register, and returns the
                      // decimal interpretation of the bitstring measured.

    // Dumps all the information about the quantum register. This has no
    // physical reality, it is only there for debugging. When verbose != 0
    // we return every value, when verbose = 0 we return only
    // probability amplitudes which differ from 0.
    void Dump(int verbose);

    // Sets state of the qubits using the arrays of complex amplitudes.
    void SetState(Complex new_state[]);

    // Sets the state to an equal superposition of all possible states
    // between 0 and number inclusive.
    void SetAverage(int number);

    // Normalise the state amplitudes.
    void Norm();

    // Get the probability of a given state. This is used in the
    // discrete Fourier transformation. In a real quantum computer such
    // an operation would not be possible, on the flip side, it would
    // also not be necessary as you could simply build a DFT gate, and
    // run your superposition through it to get the right answer.
    Complex GetProb(int state);

    // Return the size of the register.
    int Size();

private:
    int reg_size;
    Complex *State;
};

QuReg::QuReg() {
    reg_size = 0;
    State = 0;
}

// Constructor.
QuReg::QuReg(int size) {
    reg_size = size;
    State = new Complex[(int)pow(2, reg_size)];
    srand(time(NULL));
}

// Copy Constructor
QuReg::QuReg(const QuReg &old) {
    reg_size = old.reg_size;
    int reg_length = (int)pow(2, reg_size);
    State = new Complex[reg_length];
    for (int i = 0; i < reg_length; i++) {
        State[i] = old.State[i];
    }
}

// Destructor.
QuReg::~QuReg() {
    delete[] State;
}

// Return the probability amplitude of the state'th state.
Complex QuReg::GetProb(int state) {
    if (state >= pow(2, reg_size)) {
        std::cout << "You are trying to measure past the end of an array in qureg::GetProb!" << std::endl;
    } else {
        return (State[state]);
    }
}

// Normalise the probability amplitude, this ensures that the sum of
// the sum of the squares of all the real and imaginary components is
// equal to one.
void QuReg::Norm() {
    double b;
    double f, g;
    b = 0;
    for (int i = 0; i < pow(2, reg_size); i++) {
        b += pow(State[i].Real(), 2) + pow(State[i].Imaginary(), 2);
    }
    b = pow(b, -0.5);
    for (int i = 0; i < pow(2, reg_size); i++) {
        f = State[i].Real() * b;
        g = State[i].Imaginary() * b;
        State[i].Set(f, g);
    }
}

// Returns the size of the register.
int QuReg::Size() {
    return reg_size;
}

// Measure a state, and return the decimal value measured. Collapse
// the state so that the probability of measuring the measured value in
// the future is 1, and the probability of measuring any other state is 0.
int QuReg::DecMeasure() {
    int done = 0;
    int DecVal = -1; //-1 is an error, we did not measure anything.
    double rand1, a, b;
    rand1 = rand() / (double)RAND_MAX;
    a = b = 0;
    for (int i = 0; i < pow(2, reg_size); i++) {
        if (!done) {
            b += pow(State[i].Real(), 2) + pow(State[i].Imaginary(), 2);
            if (b > rand1 && rand1 > a) {
                // We have just measured the i state.
                for (int j = 0; j < pow(2, reg_size); j++) {
                    State[j].Set(0, 0);
                }
                State[i].Set(1, 0);
                DecVal = i;
                done = 1;
            }
            a += pow(State[i].Real(), 2) + pow(State[i].Imaginary(), 2);
        }
    }
    return DecVal;
}

// For debugging, output information about the register.
void QuReg::Dump(int verbose) {
    for (int i = 0; i < pow(2, reg_size); i++) {
        if (verbose || fabs(State[i].Real()) > pow(10, -14) || fabs(State[i].Imaginary()) > pow(10, -14)) {
            std::cout << "State " << i << " has probability amplitude " << State[i].Real() << " + i" << State[i].Imaginary() << std::endl;
        }
    }
}

// Set the states to those given in the new_state array.
void QuReg::SetState(Complex new_state[]) {
    // Beware, this function will cause segfaults if new_state is too small.
    for (int i = 0; i < pow(2, reg_size); i++) {
        State[i].Set(new_state[i].Real(), new_state[i].Imaginary());
    }
}

// Set the State to an equal superposition of the integers 0 -> number - 1
void QuReg::SetAverage(int number) {
    if (number >= pow(2, reg_size)) {
        std::cout << "Error, initializing past end of array in qureg::SetAverage." << std::endl;
    } else {
        double prob;
        prob = pow(number, -0.5);
        for (int i = 0; i <= number; i++) {
            State[i].Set(prob, 0);
        }
    }
}
