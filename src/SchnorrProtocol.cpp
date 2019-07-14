#include "SchnorrProtocol.hpp"


SchnorrProtocol::SchnorrProtocol(const ECGroup& ecg,
                                 const CryptoPP::ECPPoint& generator, 
                                 const CryptoPP::ECPPoint& public_key, 
                                 const CryptoPP::Integer& witness /*= 0*/)
                                 :
                                 SigmaProtocol(ecg, generator),
                                 _pub_key(public_key), 
                                 _w(witness) 
{
    _commitment.resize(1);
}


void SchnorrProtocol::generateCommitment()
{
    _u = RandomInteger(2, *_order);    
    _commitment[0] = _curve->Multiply(_u, *_gen);
}


void SchnorrProtocol::generateResponse()
{
    assert(_e > 0);

    auto we = a_times_b_mod_c(_w, _e, *_order);
    _s = (_u + we) % *_order;
}


bool SchnorrProtocol::verify()
{
    assert(_e > 0);

    auto alpha = _curve->Multiply(_s, *_gen);
    auto beta = _curve->Multiply(_e, _curve->Inverse(_pub_key));

    auto result = _curve->Add(alpha, beta);
    return (_commitment[0] == result);
}


void SchnorrProtocol::generateSimulation()
{
    assert(_e > 0);

    _s = RandomInteger(1, *_order);

    auto alpha = _curve->Multiply(_s, *_gen);
    auto beta = _curve->Multiply(_e, _curve->Inverse(_pub_key));
    _commitment[0] = _curve->Add(alpha, beta);
}


std::string SchnorrProtocol::getHashData() 
{
    std::string ret;
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_commitment[0].x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_commitment[0].y);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_pub_key.x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_pub_key.y);
    return ret;
}