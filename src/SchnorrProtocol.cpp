#include "SchnorrProtocol.hpp"


SchnorrProtocol::SchnorrProtocol(const ECGroup& ecg,
                                 const CryptoPP::ECPPoint& generator, 
                                 const CryptoPP::ECPPoint& public_key, 
                                 const CryptoPP::Integer& witness /*= 0*/)
                                 :
                                 SigmaProtocol(ecg, generator),
                                 pub_key_(public_key), 
                                 w_(witness) 
{}


void SchnorrProtocol::generateCommitment()
{
    commitment_seed_ = RandomInteger(2, order_);
    CryptoPP::ECPPoint r[1];
    r[0] = curve_.Multiply(commitment_seed_, gen_);
    // r = g^u
    transcript_.setCommitment(r, 1);
}


void SchnorrProtocol::generateResponse()
{
    assert(transcript_.challenge() > 0);

    // s = u + ew (mod q)
    auto ew = a_times_b_mod_c(w_, transcript_.challenge(), order_);
    transcript_.setResponse((commitment_seed_ + ew) % order_);
}


bool SchnorrProtocol::verify()
{
    auto a = curve_.Multiply(transcript_.response(), gen_);
    auto b = curve_.Multiply(transcript_.challenge(), 
                              curve_.Inverse(pub_key_));
    auto result = curve_.Add(a, b);
    // r = g^s * x^-e
    return (transcript_.commitment(0) == result);
}


void SchnorrProtocol::generateSimulation()
{
    assert(transcript_.challenge() > 0);

    transcript_.setResponse(RandomInteger(1, order_));

    auto a = curve_.Multiply(transcript_.response(), gen_);
    auto b = curve_.Multiply(transcript_.challenge(), 
                                 curve_.Inverse(pub_key_));
    CryptoPP::ECPPoint r[1];
    r[0] = curve_.Add(a, b);
    // r = g^s * x^-e
    transcript_.setCommitment(r, 1);
}


std::string SchnorrProtocol::getHashData() 
{
    std::string ret;
    ret += CryptoPP::IntToString<CryptoPP::Integer>(transcript_.commitment(0).x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(transcript_.commitment(0).y);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(pub_key_.x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(pub_key_.y);
    return ret;
}