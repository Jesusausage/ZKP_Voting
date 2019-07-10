#include "ElGamalProtocol.hpp"


ElGamalProtocol::ElGamalProtocol(const ECGroup& ecg,
                                 const CryptoPP::ECPPoint& generator1, 
                                 const CryptoPP::ECPPoint& generator2, 
                                 const CryptoPP::ECPPoint& public_key1, 
                                 const CryptoPP::ECPPoint& public_key2,  
                                 const CryptoPP::Integer& witness)
                                 :
                                 _curve(&ecg.curve), 
                                 _order(&ecg.order), 
                                 _gen1(generator1),
                                 _gen2(generator2),
                                 _pub_key1(public_key1),
                                 _pub_key2(public_key2), 
                                 _w(witness)
{}


ElGamalProtocol::ElGamalProtocol(const ECGroup& ecg,
                                 const CryptoPP::ECPPoint& generator1, 
                                 const CryptoPP::ECPPoint& generator2, 
                                 const CryptoPP::ECPPoint& public_key1, 
                                 const CryptoPP::ECPPoint& public_key2)
                                 :
                                 _curve(&ecg.curve), 
                                 _order(&ecg.order), 
                                 _gen1(generator1),
                                 _gen2(generator2),
                                 _pub_key1(public_key1),
                                 _pub_key2(public_key2)
{}


void ElGamalProtocol::generateCommitment()
{
    _u = RandomInteger(2, *_order);    
    _commitment1 = _curve->Multiply(_u, _gen1);
    _commitment2 = _curve->Multiply(_u, _gen2);
}


void ElGamalProtocol::generateChallenge(CryptoPP::Integer* e /*= nullptr*/)
{
    if (e) {
        _e = *e;
    }
    else {
        _e = RandomInteger(1, *_order);
    }
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

    auto alpha = _curve->Multiply(_s, _gen1);
    auto beta = _curve->Multiply(_e, _curve->Inverse(_pub_key1));
    auto result = _curve->Add(alpha, beta);
    return (_commitment1 == result);

    alpha = _curve->Multiply(_s, _gen2);
    beta = _curve->Multiply(_e, _curve->Inverse(_pub_key2));
    result = _curve->Add(alpha, beta);
    return (_commitment2 == result);
}


void ElGamalProtocol::generateSimulation()
{
    assert(_e > 0);

    _s = RandomInteger(1, *_order);

    auto alpha = _curve->Multiply(_s, _gen1);
    auto beta = _curve->Multiply(_e, _curve->Inverse(_pub_key1));
    _commitment1 = _curve->Add(alpha, beta);
    
    alpha = _curve->Multiply(_s, _gen2);
    beta = _curve->Multiply(_e, _curve->Inverse(_pub_key2));
    _commitment2 = _curve->Add(alpha, beta);
}


CryptoPP::Integer ElGamalProtocol::challengeSize()
{
    return *_order;
}


std::string ElGamalProtocol::getHashData()
{
    std::string ret;
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_commitment1.x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_commitment1.y);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_pub_key1.x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_pub_key1.y);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_commitment2.x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_commitment2.y);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_pub_key2.x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(_pub_key2.y);
    return ret;
}


std::vector<CryptoPP::ECPPoint> ElGamalProtocol::commitment()
{
    std::vector<CryptoPP::ECPPoint> commitment;
    commitment.push_back(_commitment1);
    commitment.push_back(_commitment2);
    return commitment;
}


CryptoPP::Integer ElGamalProtocol::challenge() 
{
    return _e;
}


CryptoPP::Integer ElGamalProtocol::response()
{
    return _s;
}


bool ElGamalProtocol::verifyTranscript(const Transcript& transcript) 
{
    auto alpha = _curve->Multiply(transcript.response, _gen1);
    auto beta = _curve->Multiply(transcript.challenge, 
                                 _curve->Inverse(_pub_key1));
    auto result = _curve->Add(alpha, beta);
    bool verified1 = (transcript.commitment[0] == result);

    alpha = _curve->Multiply(transcript.response, _gen2);
    beta = _curve->Multiply(transcript.challenge, 
                                 _curve->Inverse(_pub_key2));
    result = _curve->Add(alpha, beta);
    bool verified2 = (transcript.commitment[1] == result);

    return (verified1 && verified2);
}