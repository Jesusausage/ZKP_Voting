#include "OrProtocolTest.hpp"


void TestNormalOrRun()
{    
    auto ecg = GenerateECGroup();

    auto base0 = ecg.base;
    auto witness0 = RandomInteger(2, ecg.order);
    auto public_key0 = ecg.curve.Multiply(witness0, base0);
    SchnorrProtocol prot0(ecg, base0, public_key0, witness0);

    auto base1 = ecg.curve.Multiply(27, ecg.base);
    auto witness1 = RandomInteger(2, ecg.order);
    auto public_key1 = ecg.curve.Multiply(witness1, base1);
    SchnorrProtocol prot1(ecg, base1, public_key1);

    auto base2 = ecg.curve.Multiply(42, ecg.base);
    auto witness2 = RandomInteger(2, ecg.order);
    auto public_key2 = ecg.curve.Multiply(witness2, base2);
    SchnorrProtocol prot2(ecg, base2, public_key2);

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
    auto ecg = GenerateECGroup();

    auto base0 = ecg.base;
    auto witness0 = RandomInteger(2, ecg.order);
    auto public_key0 = ecg.curve.Multiply(witness0, base0);
    SchnorrProtocol prot0(ecg, base0, public_key0, witness0);

    auto base1 = ecg.curve.Multiply(27, ecg.base);
    auto witness1 = RandomInteger(2, ecg.order);
    auto public_key1 = ecg.curve.Multiply(witness1, base1);
    SchnorrProtocol prot1(ecg, base1, public_key1);

    std::vector<SigmaProtocol*> prots = {&prot0, &prot1};
    OrProtocol prot(prots, 0);

    auto nizkp = prot.generateNIZKP();
    assert(prot.verifyNIZKP(nizkp) == true);
}