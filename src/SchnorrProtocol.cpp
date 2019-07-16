#include "SchnorrProtocol.hpp"


SchnorrProtocol::SchnorrProtocol(const ECGroup& ecg,
                                 const CryptoPP::ECPPoint& generator, 
                                 const CryptoPP::ECPPoint& public_key, 
                                 const CryptoPP::Integer& witness /*= 0*/)
                                 :
                                 SigmaProtocol(ecg, generator),
                                 _pub_key(public_key), 
                                 _w(witness) 
{}


void SchnorrProtocol::generateCommitment()
{
    _commitment_seed = RandomInteger(2, *_order);
    CryptoPP::ECPPoint r[1];
    r[0] = _curve->Multiply(_commitment_seed, *_gen);
    // r = g^u
    _transcript.setCommitment(r, 1);
}


void SchnorrProtocol::generateResponse()
{
    assert(_transcript.challenge() > 0);

    // s = u + ew (mod q)
    auto ew = a_times_b_mod_c(_w, _transcript.challenge(), *_order);
    _transcript.setResponse((_commitment_seed + ew) % *_order);
}


bool SchnorrProtocol::verify()
{
    auto a = _curve->Multiply(_transcript.response(), *_gen);
    auto b = _curve->Multiply(_transcript.challenge(), 
                                 _curve->Inverse(_pub_key));
    auto result = _curve->Add(a, b);
    // r = g^s * x^-e
    auto r = _transcript.commitment();
    return (r[0] == result);
}


void SchnorrProtocol::generateSimulation()
{
    assert(_transcript.challenge() > 0);

    _transcript.setResponse(RandomInteger(1, *_order));

    auto a = _curve->Multiply(_transcript.response(), *_gen);
    auto b = _curve->Multiply(_transcript.challenge(), 
                                 _curve->Inverse(_pub_key));
    CryptoPP::ECPPoint r[1];
    r[0] = _curve->Add(a, b);
    // r = g^s * x^-e
    _transcript.setCommitment(r, 1);
}


std::string SchnorrProtocol::getHashData() 
{
    auto r = _transcript.commitment();
    std::string ret;
    ret += CryptoPP::IntToString<CryptoPP::Integer>(r[0].x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(r[0].y);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_pub_key.x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_pub_key.y);
    return ret;
}