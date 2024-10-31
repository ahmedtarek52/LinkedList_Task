#include "Complexnum.h"
Complexnum addComplex(Complexnum c1, Complexnum c2) {
    int realPart = c1.getReal() + c2.getReal();
    int imaginaryPart = c1.getImaginary() + c2.getImaginary();
    return Complexnum(realPart, imaginaryPart);
}


Complexnum subtractComplex(Complexnum c1, Complexnum c2) {
    int realPart = c1.getReal() - c2.getReal();
    int imaginaryPart = c1.getImaginary() - c2.getImaginary();
    return Complexnum(realPart, imaginaryPart);
}
