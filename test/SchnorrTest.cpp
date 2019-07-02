#include "SchnorrTest.hpp"


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


void TestSimulation() {
Group<ModularInt> G = GetGroup<ModularInt>();    
    auto public_key = G.newElement(69);

    // Peggy
    int r = 0;
    auto commitment = GenerateCommitment<ModularInt>(G, r);

    // Victor
    int challenge = GenerateChallenge(G.order());
    int response = GenerateResponse(r, challenge, 69);

    ModularInt u = GenerateSimulation(G, public_key, challenge, response);

    assert(u == commitment);
}
