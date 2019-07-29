#include "ElGamalProtocol.hpp"


ElGamalProtocol::ElGamalProtocol(const ECGroup& ecg,
                                 const CryptoPP::ECPPoint& generator1,
                                 int message)
                                 :
                                 SigmaProtocol(ecg, generator1),
                                 m_(message)
{}


void ElGamalProtocol::setParams(const CryptoPP::ECPPoint& generator2,
                                const CryptoPP::ECPPoint& public_key1,
                                const CryptoPP::ECPPoint& public_key2,
                                const CryptoPP::Integer& witness /*= 0*/)
{
    gen2_ = generator2;
    pub_key1_ = public_key1;
    pub_key2_ = public_key2;
    w_ = witness;
}


void ElGamalProtocol::generateCommitment()
{
    commitment_seed_ = RandomInteger(2, order_); 
    CryptoPP::ECPPoint r[2];   
    r[0] = curve_.Multiply(commitment_seed_, gen_);
    r[1] = curve_.Multiply(commitment_seed_, gen2_);
    // r1 = g1^u, r2 = g2^u
    transcript_.setCommitment(r, 2);
}


void ElGamalProtocol::generateResponse()
{
    assert(transcript_.challenge() > 0);

    // s = u + ew (mod q)
    auto ew = a_times_b_mod_c(w_, transcript_.challenge(), order_);
    transcript_.setResponse((commitment_seed_ + ew) % order_);
}


bool ElGamalProtocol::verify()
{
    if (!(computeCommitment1() == transcript_.commitment(0)))
        return false;
    if (!(computeCommitment2() == transcript_.commitment(1)))
        return false;

    return true;
}


void ElGamalProtocol::generateSimulation()
{
    assert(transcript_.challenge() > 0);

    transcript_.setResponse(RandomInteger(1, order_));
    CryptoPP::ECPPoint r[2];
    r[0] = computeCommitment1();    
    r[1] = computeCommitment2();
    transcript_.setCommitment(r, 2);
}


std::string ElGamalProtocol::getHashData()
{
    std::string ret;
    ret += CryptoPP::IntToString<CryptoPP::Integer>(transcript_.commitment(0).x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(transcript_.commitment(0).y);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(pub_key1_.x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(pub_key1_.y);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(transcript_.commitment(1).x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(transcript_.commitment(1).y);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(pub_key2_.x);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(pub_key2_.y);
    return ret;
}


CryptoPP::ECPPoint ElGamalProtocol::computeCommitment1()
{
    auto a = curve_.Multiply(transcript_.response(), gen_);
    auto b = curve_.Multiply(transcript_.challenge(), 
                              curve_.Inverse(pub_key1_));
    // r1 = g1^s * x1^-e
    return curve_.Add(a, b);
}


CryptoPP::ECPPoint ElGamalProtocol::computeCommitment2()
{
    auto a = curve_.Multiply(transcript_.response(), gen2_);
    auto b = curve_.Multiply(transcript_.challenge(), 
                              curve_.Inverse(pub_key2_));
    auto c = curve_.Multiply(m_*transcript_.challenge(), gen_);
    // r2 = g2^s * x2^-e * g2^me
    return curve_.Add(a, curve_.Add(b, c));
}