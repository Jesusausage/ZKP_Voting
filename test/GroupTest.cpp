#include "GroupTest.hpp"


void TestGroupConstructor() {
    ModularInt e(1, 7);
    Group<ModularInt> G(7, e);
    auto x = G.identity();

    assert(x == 1);
    assert(x == ModularInt(1, 7));
}


void TestPower() {
    ModularInt e(1, 7);
    Group<ModularInt> G(7, e);
    ModularInt x(2, 7);
    auto y = G.power(x, 3);

    assert(y == 1);
}


void TestInverse() {
    ModularInt e(1, 7);
    Group<ModularInt> G(7, e);
    ModularInt x(2, 7);
    ModularInt y = G.inverse(x);

    assert(y == 4);
}
