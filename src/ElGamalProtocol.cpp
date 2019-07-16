#include "ElGamalProtocol.hpp"


ElGamalProtocol::ElGamalProtocol(const ECGroup& ecg,
                                 const CryptoPP::ECPPoint& generator1,
                                 int message)
                                 :
                                 SigmaProtocol(ecg, generator1),
                                 _m(message)
{
    _commitment.resize(2);
}


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
    _u = RandomInteger(2, *_order);    
    _commitment[0] = _curve->Multiply(_u, *_gen);
    _commitment[1] = _curve->Multiply(_u, _gen2);
}


void ElGamalProtocol::generateResponse()
{
    assert(_e > 0);

    auto we = a_times_b_mod_c(_w, _e, *_order);
    _s = (_u + we) % *_order;
}


bool ElGamalProtocol::verify()
{
    assert(_e > 0);

    if (!(computeCommitment1() == _commitment[0]))
        return false;
    if (!(computeCommitment2() == _commitment[1]))
        return false;

    return true;
}


void ElGamalProtocol::generateSimulation()
{
    assert(_e > 0);

    _s = RandomInteger(1, *_order);
    _commitment[0] = computeCommitment1();    
    _commitment[1] = computeCommitment2();
}


std::string ElGamalProtocol::getHashData()
{
    std::string ret;
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_commitment[0].x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_commitment[0].y);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_pub_key1.x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_pub_key1.y);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_commitment[1].x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_commitment[1].y);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_pub_key2.x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_pub_key2.y);
    return ret;
}


CryptoPP::ECPPoint ElGamalProtocol::computeCommitment1()
{
    auto a = _curve->Multiply(_s, *_gen);
    auto b = _curve->Multiply(_e, _curve->Inverse(_pub_key1));

    return _curve->Add(a, b);
}


CryptoPP::ECPPoint ElGamalProtocol::computeCommitment2()
{
    auto a = _curve->Multiply(_s, _gen2);
    auto b = _curve->Multiply(_e, _curve->Inverse(_pub_key2));
    auto c = _curve->Multiply(_e * _m, *_gen);

    return _curve->Add(a, _curve->Add(b, c));
}