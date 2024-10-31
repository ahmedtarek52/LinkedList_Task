#pragma once
#include <iostream>

class Complexnum {
private:
    int real;
    int imaginary;
    static int instanceCount; 

public:
    Complexnum();
    Complexnum(const Complexnum & val);
    Complexnum(int r, int i);

    ~Complexnum();

    void setComplex(int r, int i);
    int getReal();
    int getImaginary();
    void print();
    static int getInstanceCount(); 
};


Complexnum addComplex(Complexnum c1, Complexnum c2);
Complexnum subtractComplex(Complexnum c1, Complexnum c2);
