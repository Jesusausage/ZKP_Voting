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


bool SchnorrProtocol::generateResponse()
{
    if (_e.IsZero())
        return false;

    auto we = a_times_b_mod_c(_w, _e, *_order);
    _s = (_u + we) % *_order;
    return true;
}


bool SchnorrProtocol::verify()
{
    auto alpha = _curve->Multiply(_s, *_base);
    auto beta = _curve->Multiply(_e, _curve->Inverse(_pub_key));        

    auto result = _curve->Add(alpha, beta);
    return (_commitment == result);
}


bool SchnorrProtocol::generateSimulation()
{
    if (!_e)
        return false;

    _s = RandomInteger(1, *_order);

    auto alpha = _curve->Multiply(_s, *_base);
    auto beta = _curve->Multiply(_e, _curve->Inverse(_pub_key));
    _commitment = _curve->Add(alpha, beta);
    return true;
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


std::vector<CryptoPP::ECPPoint> SchnorrProtocol::commitment()
{
    std::vector<CryptoPP::ECPPoint> commitment;
    commitment.push_back(_commitment);
    return commitment;
}


CryptoPP::Integer SchnorrProtocol::response()
{
    return _s;
}


bool SchnorrProtocol::verifyNIZKP(const NIZKP& nizkp)
{
    _commitment = nizkp.commitment[0];
    _e = nizkp.challenge;
    _s = nizkp.response;
    bool verified = this->verify();

    std::string hash_data = getHashData();
    auto challenge = genHashChallenge(hash_data);
    bool valid_e = (challenge == _e);

    return (verified && valid_e);
}