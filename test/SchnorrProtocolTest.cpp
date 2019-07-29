#include "SchnorrProtocolTest.hpp"


void TestNormalSchnorrRun() 
{   
    auto ecg = GenerateECGroup();
    auto base = GenerateECBase();
    auto witness = RandomInteger(2, ecg.order);
    auto public_key = ecg.curve.Multiply(witness, base);
    SchnorrProtocol prot(ecg, base, public_key, witness);

    prot.generateCommitment();
    prot.generateChallenge();
    prot.generateResponse();
    assert(prot.verify() == true);
}


void TestSimulatedSchnorrRun()
{    
    auto ecg = GenerateECGroup();
    auto base = GenerateECBase();
    auto witness = RandomInteger(2, ecg.order);
    auto public_key = ecg.curve.Multiply(witness, base);
    SchnorrProtocol prot(ecg, base, public_key /*, witness*/);

    prot.generateChallenge();
    prot.generateSimulation();
    assert(prot.verify() == true);
}


void TestSchnorrNIZKP()
{
    auto ecg = GenerateECGroup();
    auto base = GenerateECBase();
    auto witness = RandomInteger(2, ecg.order);
    auto public_key = ecg.curve.Multiply(witness, base);
    SchnorrProtocol prot(ecg, base, public_key, witness);

    auto nizkp = prot.generateNIZKP();
    assert(prot.verifyNIZKP(nizkp) == true);
}