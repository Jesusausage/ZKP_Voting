#include "ElGamalProtocol.hpp"


ElGamalProtocol::ElGamalProtocol(const ECGroup& ecg,
                                 const CryptoPP::ECPPoint& generator1, 
                                 const CryptoPP::ECPPoint& generator2, 
                                 const CryptoPP::ECPPoint& public_key1, 
                                 const CryptoPP::ECPPoint& public_key2,  
                                 int message,
                                 const CryptoPP::Integer& witness /*= 0*/)
                                 :
                                 _curve(&ecg.curve), 
                                 _order(&ecg.order), 
                                 _gen1(generator1),
                                 _gen2(generator2),
                                 _pub_key1(public_key1),
                                 _pub_key2(public_key2),
                                 _m(message),
                                 _w(witness)
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

    if (!(computeCommitment1() == _commitment1))
        return false;
    if (!(computeCommitment2() == _commitment2))
        return false;

    return true;
}


void ElGamalProtocol::generateSimulation()
{
    assert(_e > 0);

    _s = RandomInteger(1, *_order);
    _commitment1 = computeCommitment1();    
    _commitment2 = computeCommitment2();
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
    _commitment1 = transcript.commitment[0];
    _commitment2 = transcript.commitment[1];
    _e = transcript.challenge;
    _s = transcript.response;

    return verify();

    // auto a = _curve->Multiply(transcript.response, _gen1);
    // auto b = _curve->Multiply(transcript.challenge, 
    //                           _curve->Inverse(_pub_key1));
    // auto result = _curve->Add(a, b);
    // if (!(transcript.commitment[0] == result))
    //     return false;

    // a = _curve->Multiply(transcript.response, _gen2);
    // b = _curve->Multiply(transcript.challenge, 
    //                      _curve->Inverse(_pub_key2));
    // auto c = _curve->Multiply(transcript.challenge * _m, _gen1);
    // result = _curve->Add(a, _curve->Add(b, c));
    // if (!(transcript.commitment[1] == result))
    //     return false;

    // return true;
}


CryptoPP::ECPPoint ElGamalProtocol::computeCommitment1()
{
    auto a = _curve->Multiply(_s, _gen1);
    auto b = _curve->Multiply(_e, _curve->Inverse(_pub_key1));

    return _curve->Add(a, b);
}


CryptoPP::ECPPoint ElGamalProtocol::computeCommitment2()
{
    auto a = _curve->Multiply(_s, _gen2);
    auto b = _curve->Multiply(_e, _curve->Inverse(_pub_key2));
    auto c = _curve->Multiply(_e * _m, _gen1);

    return _curve->Add(a, _curve->Add(b, c));
}