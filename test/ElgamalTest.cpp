#include "ElgamalTest.hpp"


void TestGenerateElgamalCommitment() {
    Group<ModularInt> G = GetGroup<ModularInt>();
    int u = 0;
    auto g = G.newElement();
    auto h = G.newElement(27);
    auto commitment = GenerateCommitment<ModularInt>(G, u, g, h);
    assert(u != 0);
    
    auto r1 = G.power(g, u);
    auto r2 = G.power(h, u);
    assert(commitment.first == r1);
    assert(commitment.second == r2);
}


void TestElgamalVerification() {
    Group<ModularInt> G = GetGroup<ModularInt>();    
    int witness = 69;
    auto g = G.newElement();
    auto h = G.newElement(42);
    std::pair<ModularInt, ModularInt> public_key(G.power(g, 69), G.power(h, 69));

    // Peggy
    int u = 0;
    auto commitment = GenerateCommitment<ModularInt>(G, u, g, h);

    // Victor
    int challenge = GenerateChallenge(G.order());

    // Peggy
    int response = GenerateResponse(u, challenge, witness);

    // Victor
    bool accept =
        VerifyResponse(G, public_key, g, h, commitment, challenge, response);

    assert(accept == true);
}


void TestElgamalSimulation() {
    Group<ModularInt> G = GetGroup<ModularInt>();
    auto g = G.newElement();
    auto h = G.newElement(42);
    std::pair<ModularInt, ModularInt> public_key(G.power(g, 69), G.power(h, 69));

    // Peggy
    int u = 0;
    auto commitment = GenerateCommitment<ModularInt>(G, u, g, h);

    // Victor
    int challenge = GenerateChallenge(G.order());
    int response = GenerateResponse(u, challenge, 69);

    auto r1_r2 = GenerateSimulation(G, public_key, g, h, challenge, response);

    assert(r1_r2 == commitment);
}
