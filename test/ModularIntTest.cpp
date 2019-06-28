#include "ModularInt.hpp"
#include <assert.h>
#include <iostream>


void TestModularIntConstructor() {    
    ModularInt<7> x = 9;
    ModularInt<7> y = 2;    
    assert(x == y);
        
    ModularInt<7> a = x;
    ModularInt<7> b = -5;    
    assert(a == b);
}


void TestMultiply() {       
    ModularInt<7> x = 9;
    ModularInt<7> y = 4;
    auto z = x * y;

    assert(z == 1);
}


void TestAssignment() {       
    ModularInt<7> x = 9;

    auto y = x;
    auto z = 2;

    assert(y == x);
    assert(z == x);
}


int main() {
    TestModularIntConstructor();
    TestMultiply();
    TestAssignment();

    std::cout << "ModularInt test success.\n";

    return 0;
}
