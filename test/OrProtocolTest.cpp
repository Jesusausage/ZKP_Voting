#include "OrProtocolTest.hpp"


void TestNormalOrRun()
{
    Group<ModularInt> G0 = GetGroup<ModularInt>();
    ModularInt generator0 = G0.newElement(27);
    int witness0 = 42;
    ModularInt public_key0 = G0.power(generator0, witness0);
    SchnorrProtocol prot0(G0, generator0, public_key0, witness0);

    Group<ModularInt> G1 = GetGroup<ModularInt>();
    ModularInt generator1 = G1.newElement(28);
    ModularInt public_key1 = G1.power(generator1, 43);
    SchnorrProtocol prot1(G1, generator1, public_key1);

    OrProtocol prot(&prot0, &prot1, true);
    prot.generateCommitment();
    prot.generateChallenge();

    bool ret = prot.generateResponse();
    assert(ret == true);

    ret = prot.verify();
    assert(ret == true);
}


void TestSimulatedOrRun()
{
    Group<ModularInt> G0 = GetGroup<ModularInt>();
    ModularInt generator0 = G0.newElement(27);
    ModularInt public_key0 = G0.power(generator0, 42);
    SchnorrProtocol prot0(G0, generator0, public_key0);

    Group<ModularInt> G1 = GetGroup<ModularInt>();
    ModularInt generator1 = G1.newElement(28);
    ModularInt public_key1 = G1.power(generator1, 43);
    SchnorrProtocol prot1(G1, generator1, public_key1);

    OrProtocol prot(&prot0, &prot1, true);
    prot.generateChallenge();
    
    bool ret = prot.generateSimulation();
    assert(ret == true);

    ret = prot.verify();
    assert(ret == true);
}