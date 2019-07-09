#include "OrProtocolTest.hpp"


void TestNormalOrRun()
{    
    auto ecg0 = GenerateECGroup();
    auto witness0 = RandomInteger(2, ecg0.order);
    auto public_key0 = ecg0.curve.Multiply(witness0, 
                                                      ecg0.base);
    SchnorrProtocol prot0(ecg0, public_key0, witness0);

    auto ecg1 = GenerateECGroup();
    auto witness1 = RandomInteger(2, ecg1.order);
    auto public_key1 = ecg1.curve.Multiply(witness1, 
                                                      ecg1.base);
    SchnorrProtocol prot1(ecg1, public_key1, witness1);

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
    auto ecg0 = GenerateECGroup();
    auto witness0 = RandomInteger(2, ecg0.order);
    auto public_key0 = ecg0.curve.Multiply(witness0, 
                                                      ecg0.base);
    SchnorrProtocol prot0(ecg0, public_key0);

    auto ecg1 = GenerateECGroup();
    auto witness1 = RandomInteger(2, ecg1.order);
    auto public_key1 = ecg1.curve.Multiply(witness1, 
                                                      ecg1.base);
    SchnorrProtocol prot1(ecg1, public_key1);

    OrProtocol prot(&prot0, &prot1, true);
    prot.generateChallenge();
    
    bool ret = prot.generateSimulation();
    assert(ret == true);

    ret = prot.verify();
    assert(ret == true);
}


void TestOrNIZKP()
{
    auto ecg0 = GenerateECGroup();
    auto witness0 = RandomInteger(2, ecg0.order);
    auto public_key0 = ecg0.curve.Multiply(witness0, 
                                                      ecg0.base);
    SchnorrProtocol prot0(ecg0, public_key0, witness0);

    auto ecg1 = GenerateECGroup();
    auto witness1 = RandomInteger(2, ecg1.order);
    auto public_key1 = ecg1.curve.Multiply(witness1, 
                                                      ecg1.base);
    SchnorrProtocol prot1(ecg1, public_key1, witness1);

    OrProtocol prot(&prot0, &prot1, true);

    bool ret = prot.produceNIZKP();
    assert(ret == true); 
}