#include "OrProtocolTest.hpp"


void TestNormalOrRun()
{    
    CryptoPP::ECP curve0;
    CryptoPP::ECPPoint base0;
    CryptoPP::Integer order0;
    GenerateECGroup(curve0, base0, order0);
    CryptoPP::Integer witness0 = RandomCoeff(curve0);
    CryptoPP::ECPPoint public_key0 = curve0.Multiply(witness0, base0);
    SchnorrProtocol prot0(curve0, base0, order0, public_key0, witness0);

    CryptoPP::ECP curve1;
    CryptoPP::ECPPoint base1;
    CryptoPP::Integer order1;
    GenerateECGroup(curve1, base1, order1);
    CryptoPP::Integer witness1 = RandomCoeff(curve1);
    CryptoPP::ECPPoint public_key1 = curve1.Multiply(witness1, base1);
    SchnorrProtocol prot1(curve1, base1, order1, public_key1, witness1);

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
    CryptoPP::ECP curve0;
    CryptoPP::ECPPoint base0;
    CryptoPP::Integer order0;
    GenerateECGroup(curve0, base0, order0);
    CryptoPP::Integer witness0 = RandomCoeff(curve0);
    CryptoPP::ECPPoint public_key0 = curve0.Multiply(witness0, base0);
    SchnorrProtocol prot0(curve0, base0, order0, public_key0);

    CryptoPP::ECP curve1;
    CryptoPP::ECPPoint base1;
    CryptoPP::Integer order1;
    GenerateECGroup(curve1, base1, order1);
    CryptoPP::Integer witness1 = RandomCoeff(curve1);
    CryptoPP::ECPPoint public_key1 = curve1.Multiply(witness1, base1);
    SchnorrProtocol prot1(curve1, base1, order1, public_key1);

    OrProtocol prot(&prot0, &prot1, true);
    prot.generateChallenge();
    
    bool ret = prot.generateSimulation();
    assert(ret == true);

    ret = prot.verify();
    assert(ret == true);
}