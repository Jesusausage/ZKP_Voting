#include "OrProtocolTest.hpp"


void TestNormalOrRun()
{    
    auto elliptic_curve0 = GenerateECGroup();
    auto witness0 = RandomInteger(2, elliptic_curve0.order);
    auto public_key0 = elliptic_curve0.curve.Multiply(witness0, 
                                                      elliptic_curve0.base);
    SchnorrProtocol prot0(elliptic_curve0, public_key0, witness0);

    auto elliptic_curve1 = GenerateECGroup();
    auto witness1 = RandomInteger(2, elliptic_curve1.order);
    auto public_key1 = elliptic_curve1.curve.Multiply(witness1, 
                                                      elliptic_curve1.base);
    SchnorrProtocol prot1(elliptic_curve1, public_key1, witness1);

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
    auto elliptic_curve0 = GenerateECGroup();
    auto witness0 = RandomInteger(2, elliptic_curve0.order);
    auto public_key0 = elliptic_curve0.curve.Multiply(witness0, 
                                                      elliptic_curve0.base);
    SchnorrProtocol prot0(elliptic_curve0, public_key0);

    auto elliptic_curve1 = GenerateECGroup();
    auto witness1 = RandomInteger(2, elliptic_curve1.order);
    auto public_key1 = elliptic_curve1.curve.Multiply(witness1, 
                                                      elliptic_curve1.base);
    SchnorrProtocol prot1(elliptic_curve1, public_key1);

    OrProtocol prot(&prot0, &prot1, true);
    prot.generateChallenge();
    
    bool ret = prot.generateSimulation();
    assert(ret == true);

    ret = prot.verify();
    assert(ret == true);
}


void TestOrNIZKP()
{
    auto elliptic_curve0 = GenerateECGroup();
    auto witness0 = RandomInteger(2, elliptic_curve0.order);
    auto public_key0 = elliptic_curve0.curve.Multiply(witness0, 
                                                      elliptic_curve0.base);
    SchnorrProtocol prot0(elliptic_curve0, public_key0, witness0);

    auto elliptic_curve1 = GenerateECGroup();
    auto witness1 = RandomInteger(2, elliptic_curve1.order);
    auto public_key1 = elliptic_curve1.curve.Multiply(witness1, 
                                                      elliptic_curve1.base);
    SchnorrProtocol prot1(elliptic_curve1, public_key1, witness1);

    OrProtocol prot(&prot0, &prot1, true);
    
    bool ret = prot.produceNIZKP();
    assert(ret == true); 
}