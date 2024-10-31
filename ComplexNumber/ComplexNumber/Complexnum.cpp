#include "Complexnum.h"

int Complexnum::instanceCount = 0;
Complexnum::Complexnum() : real(0), imaginary(0) {
    instanceCount++;
}

Complexnum::Complexnum(const Complexnum& val)
{
    real = val.real;
    imaginary = val.imaginary;
    instanceCount++;
}

Complexnum::Complexnum(int r, int i) : real(r), imaginary(i) {
    instanceCount++;
}


Complexnum::~Complexnum() {
    instanceCount -- ;
}

void Complexnum::setComplex(int r, int i) {
    real = r;
    imaginary = i;
}

int Complexnum::getReal() {
    return real;
}

int Complexnum::getImaginary() {
    return imaginary;
}

void Complexnum::print() {
    std::cout << real << " + " << imaginary << "i" << std::endl;
}

int Complexnum::getInstanceCount() {
    return instanceCount; 
}

