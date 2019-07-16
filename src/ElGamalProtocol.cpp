#include "ElGamalProtocol.hpp"


ElGamalProtocol::ElGamalProtocol(const ECGroup& ecg,
                                 const CryptoPP::ECPPoint& generator1,
                                 int message)
                                 :
                                 SigmaProtocol(ecg, generator1),
                                 _m(message)
{}


void ElGamalProtocol::setParams(const CryptoPP::ECPPoint& generator2,
                                const CryptoPP::ECPPoint& public_key1,
                                const CryptoPP::ECPPoint& public_key2,
                                const CryptoPP::Integer& witness /*= 0*/)
{
    _gen2 = generator2;
    _pub_key1 = public_key1;
    _pub_key2 = public_key2;
    _w = witness;
}


void ElGamalProtocol::generateCommitment()
{
    _commitment_seed = RandomInteger(2, *_order); 
    CryptoPP::ECPPoint r[2];   
    r[0] = _curve->Multiply(_commitment_seed, *_gen);
    r[1] = _curve->Multiply(_commitment_seed, _gen2);
    // r1 = g1^u, r2 = g2^u
    _transcript.setCommitment(r, 2);
}


void ElGamalProtocol::generateResponse()
{
    assert(_transcript.challenge() > 0);

    // s = u + ew (mod q)
    auto ew = a_times_b_mod_c(_w, _transcript.challenge(), *_order);
    _transcript.setResponse((_commitment_seed + ew) % *_order);
}


bool ElGamalProtocol::verify()
{
    auto r = _transcript.commitment();
    if (!(computeCommitment1() == r[0]))
        return false;
    if (!(computeCommitment2() == r[1]))
        return false;

    return true;
}


void ElGamalProtocol::generateSimulation()
{
    assert(_transcript.challenge() > 0);

    _transcript.setResponse(RandomInteger(1, *_order));
    CryptoPP::ECPPoint r[2];
    r[0] = computeCommitment1();    
    r[1] = computeCommitment2();
    _transcript.setCommitment(r, 2);
}


std::string ElGamalProtocol::getHashData()
{
    auto r = _transcript.commitment();
    std::string ret;
    ret += CryptoPP::IntToString<CryptoPP::Integer>(r[0].x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(r[0].y);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_pub_key1.x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_pub_key1.y);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(r[1].x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(r[1].y);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_pub_key2.x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_pub_key2.y);
    return ret;
}


CryptoPP::ECPPoint ElGamalProtocol::computeCommitment1()
{
    auto a = _curve->Multiply(_transcript.response(), *_gen);
    auto b = _curve->Multiply(_transcript.challenge(), 
                              _curve->Inverse(_pub_key1));
    // r1 = g1^s * x1^-e
    return _curve->Add(a, b);
}


CryptoPP::ECPPoint ElGamalProtocol::computeCommitment2()
{
    auto a = _curve->Multiply(_transcript.response(), _gen2);
    auto b = _curve->Multiply(_transcript.challenge(), 
                              _curve->Inverse(_pub_key2));
    auto c = _curve->Multiply(_m*_transcript.challenge(), *_gen);
    // r2 = g2^s * x2^-e * g2^me
    return _curve->Add(a, _curve->Add(b, c));
}