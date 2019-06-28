#include "Group.hpp"
#include <assert.h>
#include <iostream>


void TestGroupConstructor() {
    Group<ModularInt<7>> G(7);
    auto e = G.identity();

    assert(e == 1);
    assert(e == ModularInt<7>(1));
}


void TestPower() {
    Group<ModularInt<7>> G(7);
    ModularInt<7> x = 2;
    auto y = G.power(x, 3);

    assert(y == 1);
}


void TestInverse() {
    Group<ModularInt<7>> G(7);
    ModularInt<7> x = 2;
    auto y = G.inverse(x);

    assert(y == 4);
}


int main() {
    TestGroupConstructor();
    TestPower();
    TestInverse();

    std::cout << "Group test success.\n";

    return 0;
}
