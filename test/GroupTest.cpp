#include "GroupTest.hpp"


void TestGroupConstructor() {
    ModularInt g(2, 7);
    Group<ModularInt> G(g, 7);
    auto x = G.identity();
    assert(x == 1);
    assert(x == ModularInt(1, 7));

    auto y = G.newElement(2);
    assert(y == 4);
    assert(y == ModularInt(4, 7));
}


void TestPower() {
    ModularInt g(2, 7);
    Group<ModularInt> G(g, 7);
    auto x = G.newElement();
    auto y = G.power(x, 4);
    assert(y == 2);
    assert(y == ModularInt(2, 7));

    auto z = G.power(x, -4);
    assert(z == 4);
    assert(z == ModularInt(4, 7));

    auto product = y * z;
    assert(product == G.identity());

    auto a = G.power(x, 3);
    auto b = G.power(x, 21);
    auto c = G.power(x, -15);
    assert(a == b);
    assert(a == c);
}


void TestInverse() {
    ModularInt g(2, 7);
    Group<ModularInt> G(g, 7);
    auto x = G.newElement();
    auto y = G.inverse(x);
    assert(y == 4);
    assert(y == ModularInt(4, 7));
}
