#include "ModularIntTest.hpp"


void TestModularIntConstructor() {    
    ModularInt x(9, 7);
    ModularInt y(2, 7);    
    assert(x == y);
        
    ModularInt a(x);
    ModularInt b(-5, 7);
    assert(a == b);

    ModularInt c(3, 7);
    c = a;
    assert(c == b);
}


void TestMultiply() {       
    ModularInt x(9, 7);
    ModularInt y(4, 7);
    auto z = x * y;
    assert(z == 1);
}


void TestAssignment() {       
    ModularInt x(9, 7);
    auto y = x;
    ModularInt z(2, 7);
    assert(y == x);
    assert(z == x);
}
