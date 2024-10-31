#include "Complexnum.h"
#include <iostream>
using namespace std;



int main() {
    Complexnum c1(4, 3);
    Complexnum c2(2, 1);
    Complexnum c3(2, 1);
    Complexnum c4(2, 1);



    Complexnum resultAdd = addComplex(c1, c2);
    Complexnum resultSubtract = subtractComplex(c1, c2);

    std::cout << "Addition: ";
    resultAdd.print();

    std::cout << "Subtraction: ";
    resultSubtract.print();

    std::cout << "Current number of instances: " << Complexnum::getInstanceCount() << std::endl;

    return 0;
}
