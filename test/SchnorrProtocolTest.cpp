#include "SchnorrProtocolTest.hpp"


void TestNormalSchnorrRun() 
{   
    Group<ModularInt> G = GetGroup<ModularInt>();
    ModularInt generator = G.newElement(27);
    int witness = 42;
    ModularInt public_key = G.power(generator, witness);

    SchnorrProtocol prot(G, generator, public_key, witness);
    prot.generateCommitment();
    prot.generateChallenge();

    bool ret = prot.generateResponse();
    assert(ret == true);

    ret = prot.verify();
    assert(ret == true);
}


void TestSimulatedSchnorrRun()
{
    Group<ModularInt> G = GetGroup<ModularInt>();
    ModularInt generator = G.newElement(27);
    ModularInt public_key = G.power(generator, 42);

    SchnorrProtocol prot(G, generator, public_key);
    prot.generateChallenge();

    bool ret = prot.generateSimulation();
    assert(ret == true);

    ret = prot.verify();
    assert(ret == true);
}