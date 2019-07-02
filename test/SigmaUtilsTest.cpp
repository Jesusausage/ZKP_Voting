#include "SigmaUtilsTest.hpp"


void TestGetPrime() {
    int p = GetPrime();

    double ceil = sqrt(p);
    for (int i = 2; i < ceil; i++) {
        assert(p % i != 0);
    }
}


void TestGetGroup() {
    Group<ModularInt> G = GetGroup<ModularInt>();

    auto x = G.newElement();
    auto y = G.power(x, G.order());
    assert(y == G.identity());

    auto a = G.newElement(31);
    auto b = G.power(a, 42);
    auto c = G.power(a, -42);
    auto d = b * c;
    assert(d == G.identity());
    assert(G.inverse(b) == c);
}
