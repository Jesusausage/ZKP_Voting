#include "ElGamalProtocolTest.hpp"


void TestNormalElGamalRun() 
{   
    auto ecg = GenerateECGroup();

    auto gen1 = ecg.base;
    auto gen2 = ecg.curve.Multiply(27, ecg.base);
    auto witness = RandomInteger(2, ecg.order);
    auto public_key1 = ecg.curve.Multiply(witness, gen1);
    auto public_key2 = ecg.curve.Add(gen1, ecg.curve.Multiply(witness, gen2));
    ElGamalProtocol prot(ecg, gen1, gen2, 1, public_key1, public_key2, witness);

    prot.generateCommitment();
    prot.generateChallenge();
    prot.generateResponse();
    assert(prot.verify() == true);
}


void TestSimulatedElGamalRun()
{
    auto ecg = GenerateECGroup();

    auto gen1 = ecg.base;
    auto gen2 = ecg.curve.Multiply(27, ecg.base);
    auto witness = RandomInteger(2, ecg.order);
    auto public_key1 = ecg.curve.Multiply(witness, gen1);
    auto public_key2 = ecg.curve.Multiply(witness, gen2);
    ElGamalProtocol prot(ecg, gen1, gen2, 0, public_key1, public_key2
                         /*, witness*/);

    prot.generateChallenge();
    prot.generateSimulation();
    assert(prot.verify() == true);
}


void TestElGamalNIZKP()
{
    auto ecg = GenerateECGroup();

    auto gen1 = ecg.base;
    auto gen2 = ecg.curve.Multiply(27, ecg.base);
    auto witness = RandomInteger(2, ecg.order);
    auto public_key1 = ecg.curve.Multiply(witness, gen1);
    auto public_key2 = ecg.curve.Multiply(witness, gen2);
    ElGamalProtocol prot(ecg, gen1, gen2, 0, public_key1, public_key2, witness);

    auto nizkp = prot.generateNIZKP();
    assert(prot.verifyNIZKP(nizkp) == true);
}