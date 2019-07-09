#include "SchnorrProtocol.hpp"


SchnorrProtocol::SchnorrProtocol(const ECGroup& ecg,
                                 const CryptoPP::ECPPoint& public_key, 
                                 const CryptoPP::Integer& witness)
                                 :
                                 _curve(&ecg.curve), 
                                 _base(&ecg.base),
                                 _order(&ecg.order),
                                 _pub_key(public_key), 
                                 _w(witness) 
{}


SchnorrProtocol::SchnorrProtocol(const ECGroup& ecg,
                                 const CryptoPP::ECPPoint& public_key)
                                 :
                                 _curve(&ecg.curve), 
                                 _base(&ecg.base),
                                 _order(&ecg.order),
                                 _pub_key(public_key)
{}


void SchnorrProtocol::generateCommitment()
{
    _u = RandomInteger(2, *_order);    
    _commitment = _curve->Multiply(_u, *_base);
}


void SchnorrProtocol::generateChallenge(CryptoPP::Integer* e /*= nullptr*/)
{
    if (e) {
        _e = *e;
    }
    else {
        _e = RandomInteger(1, *_order);
    }
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

    auto alpha = _curve->Multiply(_s, *_base);
    auto beta = _curve->Multiply(_e, _curve->Inverse(_pub_key));        

    auto result = _curve->Add(alpha, beta);
    return (_commitment == result);
}


void SchnorrProtocol::generateSimulation()
{
    assert(_e > 0);

    _s = RandomInteger(1, *_order);

    auto alpha = _curve->Multiply(_s, *_base);
    auto beta = _curve->Multiply(_e, _curve->Inverse(_pub_key));
    _commitment = _curve->Add(alpha, beta);
}


std::string SchnorrProtocol::getHashData() 
{
    std::string ret;
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_commitment.x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_commitment.y);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_pub_key.x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_pub_key.y);
    return ret;
}


void SchnorrProtocol::generateNIZKP()
{
    generateCommitment();

    std::string hash_data = getHashData();
    auto hash_challenge = _genHashChallenge(hash_data);
    generateChallenge(&hash_challenge);
    generateResponse();
    assert(verify() == true);

    _nizkp = {commitment(), challenge(), response()};
}


bool SchnorrProtocol::verifyNIZKP(const SchnorrNIZKP& nizkp)
{
    _commitment = nizkp.commitment;
    _e = nizkp.challenge;
    _s = nizkp.response;
    bool verified = this->verify();

    std::string hash_data = getHashData();
    auto challenge = _genHashChallenge(hash_data);
    bool valid_e = (challenge == _e);

    return (verified && valid_e);
}