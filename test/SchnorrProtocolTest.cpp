#include "SchnorrProtocolTest.hpp"


void TestNormalSchnorrRun() 
{   
    auto elliptic_curve = GenerateECGroup();
    auto witness = RandomInteger(2, elliptic_curve.order);
    auto public_key = elliptic_curve.curve.Multiply(witness, 
                                                    elliptic_curve.base);
    SchnorrProtocol prot(elliptic_curve, public_key, witness);

    prot.generateCommitment();
    prot.generateChallenge();
    bool ret = prot.generateResponse();
    assert(ret == true);

    ret = prot.verify();
    assert(ret == true);
}


void TestSimulatedSchnorrRun()
{    
    auto elliptic_curve = GenerateECGroup();
    auto witness = RandomInteger(2, elliptic_curve.order);
    auto public_key = elliptic_curve.curve.Multiply(witness, 
                                                    elliptic_curve.base);
    SchnorrProtocol prot(elliptic_curve, public_key, witness);

    prot.generateChallenge();
    bool ret = prot.generateSimulation();
    assert(ret == true);

    ret = prot.verify();
    assert(ret == true);
}


void TestSchnorrNIZKP()
{
    auto elliptic_curve = GenerateECGroup();
    auto witness = RandomInteger(2, elliptic_curve.order);
    auto public_key = elliptic_curve.curve.Multiply(witness, 
                                                    elliptic_curve.base);
    SchnorrProtocol prot(elliptic_curve, public_key, witness);

    bool ret = prot.produceNIZKP();
    assert(ret == true); 
}