#include "SigmaProtocol.hpp"


SigmaProtocol::SigmaProtocol(const ECGroup& ecg, 
                             const CryptoPP::ECPPoint& generator)
                             :
                             _curve(&ecg.curve),
                             _order(&ecg.order),
                             _gen(&generator)
{}


void SigmaProtocol::generateChallenge(CryptoPP::Integer* e /*= nullptr*/)
{
    if (e) {
        _e = *e;
    }
    else {
        _e = RandomInteger(1, *_order);
    }
}


CryptoPP::Integer SigmaProtocol::challengeSize()
{
    return *_order;
}


std::vector<CryptoPP::ECPPoint> SigmaProtocol::commitment()
{
    return _commitment;
}


CryptoPP::Integer SigmaProtocol::challenge() 
{
    return _e;
}


CryptoPP::Integer SigmaProtocol::response()
{
    return _s;
}


Transcript SigmaProtocol::getTranscript()
{
    return {commitment(), challenge(), response()};
}


void SigmaProtocol::setTranscript(const Transcript& transcript) 
{
    _commitment = transcript.commitment;
    _e = transcript.challenge;
    _s = transcript.response;
}


Transcript SigmaProtocol::generateNIZKP()
{
    generateCommitment();
    auto hash_e = GenHashChallenge(getHashData(), challengeSize());
    generateChallenge(&hash_e);
    generateResponse();

    return {commitment(), challenge(), response()};
}


bool SigmaProtocol::verifyNIZKP(const Transcript& nizkp)
{
    setTranscript(nizkp);

    if (verify() == false)
        return false;

    auto hash_e = GenHashChallenge(getHashData(), challengeSize());
    if (hash_e != challenge())
        return false;

    return true;
}


CryptoPP::Integer GenHashChallenge(const std::string& hash_data,
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


CompressedTranscript CompressTranscript(const Transcript& transcript)
{
    CompressedTranscript ret;
    for (auto com : transcript.commitment) {
        ret.commitment.push_back(CompressPoint(com));
    }
    ret.challenge = transcript.challenge;
    ret.response = transcript.response;

    return ret;
}


Transcript DecompressTranscript(const CompressedTranscript& compressed_transcript,
                                const CryptoPP::ECP& curve)
{
    Transcript ret;
    for (auto com : compressed_transcript.commitment) {
        ret.commitment.push_back(DecompressPoint(com, curve));
    }
    ret.challenge = compressed_transcript.challenge;
    ret.response = compressed_transcript.response;

    return ret;
}