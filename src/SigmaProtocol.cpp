#include "SigmaProtocol.hpp"


SigmaProtocol::SigmaProtocol(const ECGroup& ecg, 
                             const CryptoPP::ECPPoint& generator)
                             :
                             curve_(&ecg.curve),
                             order_(&ecg.order),
                             gen_(&generator)
{}


void SigmaProtocol::generateChallenge(CryptoPP::Integer* e /*= nullptr*/)
{
    if (e) {
        transcript_.setChallenge(*e);
    }
    else {
        transcript_.setChallenge(RandomInteger(1, *order_));
    }
}


CryptoPP::Integer SigmaProtocol::challenge() const
{
    return transcript_.challenge();
}


CryptoPP::Integer SigmaProtocol::challengeSize() const
{
    return *order_;
}


Transcript SigmaProtocol::getTranscript() const
{
    return transcript_;
}


void SigmaProtocol::setTranscript(const Transcript& transcript) 
{
    transcript_ = transcript;
}


Transcript SigmaProtocol::generateNIZKP()
{
    generateCommitment();
    auto hash_e = GenHashChallenge(getHashData(), *order_);
    generateChallenge(&hash_e);
    generateResponse();

    return transcript_;
}


bool SigmaProtocol::verifyNIZKP(const Transcript& nizkp)
{
    setTranscript(nizkp);

    if (verify() == false)
        return false;

    auto hash_e = GenHashChallenge(getHashData(), *order_);
    if (hash_e != transcript_.challenge())
        return false;

    return true;
}


CryptoPP::Integer GenHashChallenge(std::string hash_data,
                                   const CryptoPP::Integer& challenge_max)
{
    const auto challenge_size = challenge_max.ByteCount();
    CryptoPP::byte* digest = new CryptoPP::byte[challenge_size];

    CryptoPP::SHA3_256 hash;
    hash.Update((CryptoPP::byte*)hash_data.data(), hash_data.size());
    hash.TruncatedFinal(digest, challenge_size);

    auto decoded_int = CryptoPP::Integer(digest, challenge_size);
    delete [] digest;
    return decoded_int % challenge_max;
}