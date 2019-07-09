#include "SchnorrProtocolTest.hpp"


void TestNormalSchnorrRun() 
{   
    auto ecg = GenerateECGroup();
    auto witness = RandomInteger(2, ecg.order);
    auto public_key = ecg.curve.Multiply(witness, ecg.base);
    SchnorrProtocol prot(ecg, public_key, witness);

    prot.generateCommitment();
    prot.generateChallenge();
    prot.generateResponse();
    assert(prot.verify() == true);
}


void TestSimulatedSchnorrRun()
{    
    auto ecg = GenerateECGroup();
    auto witness = RandomInteger(2, ecg.order);
    auto public_key = ecg.curve.Multiply(witness, ecg.base);
    SchnorrProtocol prot(ecg, public_key /*, witness*/);

    prot.generateChallenge();
    prot.generateSimulation();
    assert(prot.verify() == true);
}


void TestSchnorrNIZKP()
{
    auto ecg = GenerateECGroup();
    auto witness = RandomInteger(2, ecg.order);
    auto public_key = ecg.curve.Multiply(witness, ecg.base);
    SchnorrProtocol prot(ecg, public_key, witness);

    prot.generateNIZKP();
    auto nizkp = prot.getNIZKP();
    assert(prot.verifyNIZKP(nizkp) == true);
}