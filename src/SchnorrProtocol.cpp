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
    std::vector<CryptoPP::ECPPoint> commitment_vec;
    commitment_vec.push_back(commitment());

    std::string hash_data = getHashData();
    auto hash_challenge = _genHashChallenge(hash_data);
    generateChallenge(&hash_challenge);
    generateResponse();
    assert(verify() == true);

    _nizkp = {commitment_vec, challenge(), response()};
}


bool SchnorrProtocol::verifyNIZKP(const Transcript& nizkp)
{
    _commitment = nizkp.commitment[0];
    _e = nizkp.challenge;
    _s = nizkp.response;
    if (verify() == false)
        return false;

    std::string hash_data = getHashData();
    auto hash_challenge = _genHashChallenge(hash_data);
    if (hash_challenge != _e)
        return false;

    return true;
}


Transcript SchnorrProtocol::getTranscript() 
{
    std::vector<CryptoPP::ECPPoint> commitment_vec;
    commitment_vec.push_back(_commitment);
    return { commitment_vec, _e, _s };
}


bool SchnorrProtocol::verifyTranscript(const Transcript& transcript) 
{
    auto alpha = _curve->Multiply(transcript.response, *_base);
    auto beta = _curve->Multiply(transcript.challenge, 
                                 _curve->Inverse(_pub_key));

    auto result = _curve->Add(alpha, beta);
    return (transcript.commitment[0] == result);
}