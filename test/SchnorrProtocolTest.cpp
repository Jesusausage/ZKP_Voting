#include "SchnorrProtocolTest.hpp"


void TestNormalSchnorrRun() 
{   
    auto ecg = GenerateECGroup();
    auto witness = RandomInteger(2, ecg.order);
    auto public_key = ecg.curve.Multiply(witness, 
                                                    ecg.base);
    SchnorrProtocol prot(ecg, public_key, witness);

    prot.generateCommitment();
    prot.generateChallenge();
    bool ret = prot.generateResponse();
    assert(ret == true);

    ret = prot.verify();
    assert(ret == true);
}


void TestSimulatedSchnorrRun()
{    
    auto ecg = GenerateECGroup();
    auto witness = RandomInteger(2, ecg.order);
    auto public_key = ecg.curve.Multiply(witness, 
                                                    ecg.base);
    SchnorrProtocol prot(ecg, public_key, witness);

    prot.generateChallenge();
    bool ret = prot.generateSimulation();
    assert(ret == true);

    ret = prot.verify();
    assert(ret == true);
}


void TestSchnorrNIZKP()
{
    auto ecg = GenerateECGroup();
    auto witness = RandomInteger(2, ecg.order);
    auto public_key = ecg.curve.Multiply(witness, 
                                                    ecg.base);
    SchnorrProtocol prot(ecg, public_key, witness);

    bool ret = prot.produceNIZKP();
    assert(ret == true); 
}