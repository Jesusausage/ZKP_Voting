#include "OrProtocolTest.hpp"


void TestNormalOrRun()
{    
    auto ecg0 = GenerateECGroup();
    auto witness0 = RandomInteger(2, ecg0.order);
    auto public_key0 = ecg0.curve.Multiply(witness0, ecg0.base);
    SchnorrProtocol prot0(ecg0, public_key0, witness0);

    auto ecg1 = GenerateECGroup();
    auto witness1 = RandomInteger(2, ecg1.order);
    auto public_key1 = ecg1.curve.Multiply(witness1, ecg1.base);
    SchnorrProtocol prot1(ecg1, public_key1);

    auto ecg2 = GenerateECGroup();
    auto witness2 = RandomInteger(2, ecg2.order);
    auto public_key2 = ecg2.curve.Multiply(witness2, ecg2.base);
    SchnorrProtocol prot2(ecg2, public_key1);

    std::vector<SigmaProtocol*> prots = {&prot0, &prot1, &prot2};
    OrProtocol prot(prots, 0);
    prot.generateCommitment();
    auto challenge = RandomInteger(2 * prot0.challengeSize(), 
                                   3 * prot0.challengeSize());
    prot.generateChallenge(challenge);
    prot.generateResponse();
    assert(prot.verify() == true);
}


void TestOrNIZKP()
{
    auto ecg0 = GenerateECGroup();
    auto witness0 = RandomInteger(2, ecg0.order);
    auto public_key0 = ecg0.curve.Multiply(witness0, ecg0.base);
    SchnorrProtocol prot0(ecg0, public_key0, witness0);

    auto ecg1 = GenerateECGroup();
    auto witness1 = RandomInteger(2, ecg1.order);
    auto public_key1 = ecg1.curve.Multiply(witness1, ecg1.base);
    SchnorrProtocol prot1(ecg1, public_key1);

    std::vector<SigmaProtocol*> prots = {&prot0, &prot1};
    OrProtocol prot(prots, 0);

    prot.generateNIZKP();
    auto nizkp = prot.getNIZKP();
    assert(prot.verifyNIZKP(nizkp) == true);
}