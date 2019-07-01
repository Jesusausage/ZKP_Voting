#include "SchnorrTest.hpp"


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


void TestGenerateCommitment() {    
    Group<ModularInt> G = GetGroup<ModularInt>();
    int r = 0;
    auto u = GenerateCommitment<ModularInt>(G, r);
    assert(r != 0);
    
    auto v = G.power(G.generator(), r);
    assert(u == v);
}


void TestVerification() {
    Group<ModularInt> G = GetGroup<ModularInt>();    
    int witness = 69;
    auto public_key = G.newElement(69);

    // Peggy
    int r = 0;
    auto commitment = GenerateCommitment<ModularInt>(G, r);

    // Victor
    int challenge = GenerateChallenge(G.order());

    // Peggy
    int response = GenerateResponse(r, challenge, witness);

    // Victor
    bool accept =
	VerifyResponse(G, public_key, commitment, challenge, response);

    assert(accept == true);
}
