#include "OrProtocolTest.hpp"


void TestSchnorrOrRun()
{    
    auto ecg = GenerateECGroup();
    auto base = GenerateECBase();

    auto gen0 = base;
    auto witness0 = RandomInteger(2, ecg.order);
    auto public_key0 = ecg.curve.Multiply(witness0, gen0);
    SchnorrProtocol prot0(ecg, gen0, public_key0, witness0);

    auto gen1 = ecg.curve.Multiply(27, base);
    auto witness1 = RandomInteger(2, ecg.order);
    auto public_key1 = ecg.curve.Multiply(witness1, gen1);
    SchnorrProtocol prot1(ecg, gen1, public_key1);

    auto gen2 = ecg.curve.Multiply(42, base);
    auto witness2 = RandomInteger(2, ecg.order);
    auto public_key2 = ecg.curve.Multiply(witness2, gen2);
    SchnorrProtocol prot2(ecg, gen2, public_key2);

    SigmaProtocol* prots[3] = {&prot0, &prot1, &prot2};
    OrProtocol prot(prots, 3, 0);
    prot.generateCommitment();
    prot.generateChallenge(RandomInteger(1, prot0.challengeSize()));
    prot.generateResponse();
    assert(prot.verify() == true);
}


void TestElGamalOrRun()
{ 
    auto ecg = GenerateECGroup();
    auto base = GenerateECBase();

    auto gen1 = base;
    auto gen2 = ecg.curve.Multiply(27, base);
    auto witness = RandomInteger(2, ecg.order);
    auto public_key1 = ecg.curve.Multiply(witness, gen1);
    auto public_key2 = ecg.curve.Multiply(witness, gen2);
    // auto public_key2 = ecg.curve.Add(gen1, ecg.curve.Multiply(witness, gen2));
    ElGamalProtocol prot0(ecg, gen1, 0);
    prot0.setParams(gen2, public_key1, public_key2, witness);
    ElGamalProtocol prot1(ecg, gen1, 1);
    prot1.setParams(gen2, public_key1, public_key2);

    SigmaProtocol* prots[2] = {&prot0, &prot1};
    OrProtocol prot(prots, 2, 0);
    prot.generateCommitment();
    prot.generateChallenge(RandomInteger(1, prot0.challengeSize()));
    prot.generateResponse();
    assert(prot.verify() == true);
}


void TestSchnorrOrNIZKP()
{
    auto ecg = GenerateECGroup();
    auto base = GenerateECBase();

    auto gen0 = base;
    auto witness0 = RandomInteger(2, ecg.order);
    auto public_key0 = ecg.curve.Multiply(witness0, gen0);
    SchnorrProtocol prot0(ecg, gen0, public_key0, witness0);

    auto gen1 = ecg.curve.Multiply(27, base);
    auto witness1 = RandomInteger(2, ecg.order);
    auto public_key1 = ecg.curve.Multiply(witness1, gen1);
    SchnorrProtocol prot1(ecg, gen1, public_key1);

    SigmaProtocol* prots[2] = {&prot0, &prot1};
    OrProtocol prot(prots, 2, 0);

    auto nizkp = prot.generateNIZKP();
    assert(prot.verifyNIZKP(nizkp) == true);
}


void TestElGamalOrNIZKP()
{ 
    auto ecg = GenerateECGroup();
    auto base = GenerateECBase();

    auto gen1 = base;
    auto gen2 = ecg.curve.Multiply(27, base);
    auto witness = RandomInteger(2, ecg.order);
    auto public_key1 = ecg.curve.Multiply(witness, gen1);
    // auto public_key2 = ecg.curve.Multiply(witness, gen2);
    auto public_key2 = ecg.curve.Add(gen1, ecg.curve.Multiply(witness, gen2));
    ElGamalProtocol prot0(ecg, gen1, 0);
    prot0.setParams(gen2, public_key1, public_key2);
    ElGamalProtocol prot1(ecg, gen1, 1);
    prot1.setParams(gen2, public_key1, public_key2, witness);

    SigmaProtocol* prots[2] = {&prot0, &prot1};
    OrProtocol prot(prots, 2, 1);

    auto nizkp = prot.generateNIZKP();
    assert(prot.verifyNIZKP(nizkp) == true);
}