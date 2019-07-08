#include "SchnorrProtocolTest.hpp"


void TestNormalSchnorrRun() 
{   
    CryptoPP::ECP curve;
    CryptoPP::ECPPoint base;
    CryptoPP::Integer order;
    GenerateECGroup(curve, base, order);

    CryptoPP::Integer a = RandomCoeff(curve);
    CryptoPP::Integer b = RandomCoeff(curve);
    CryptoPP::Integer c = (a * b) % order;
    auto A = curve.Multiply(a, base);
    auto B = curve.Multiply(b, A);
    auto C = curve.Multiply(c, base);
    assert(B == C);

    CryptoPP::Integer witness = RandomCoeff(curve);
    CryptoPP::ECPPoint public_key = curve.Multiply(witness, base);

    SchnorrProtocol prot(curve, base, order, public_key, witness);
    prot.generateCommitment();
    prot.generateChallenge();

    bool ret = prot.generateResponse();
    assert(ret == true);

    ret = prot.verify();
    assert(ret == true);
}


void TestSimulatedSchnorrRun()
{    
    CryptoPP::ECP curve;
    CryptoPP::ECPPoint base;
    CryptoPP::Integer order;
    GenerateECGroup(curve, base, order);
    CryptoPP::Integer witness = RandomCoeff(curve);
    CryptoPP::ECPPoint public_key = curve.Multiply(witness, base);

    SchnorrProtocol prot(curve, base, order, public_key);
    prot.generateChallenge();

    bool ret = prot.generateSimulation();
    assert(ret == true);

    ret = prot.verify();
    assert(ret == true);
}