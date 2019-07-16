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
        _transcript.setChallenge(*e);
    }
    else {
        _transcript.setChallenge(RandomInteger(1, *_order));
    }
}


CryptoPP::Integer SigmaProtocol::challengeSize() const
{
    return *_order;
}


CryptoPP::ECPPoint* SigmaProtocol::commitment() const
{
    return _transcript.commitment();
}


int SigmaProtocol::commitmentSize() const
{
    return _transcript.commitmentSize();
}


CryptoPP::Integer SigmaProtocol::challenge() const
{
    return _transcript.challenge();
}


CryptoPP::Integer SigmaProtocol::response() const
{
    return _transcript.response();
}


Transcript SigmaProtocol::getTranscript() const
{
    return _transcript;
}


void SigmaProtocol::setTranscript(const Transcript& transcript) 
{
    _transcript = transcript;
}


Transcript SigmaProtocol::generateNIZKP()
{
    generateCommitment();
    auto hash_e = GenHashChallenge(getHashData(), *_order);
    generateChallenge(&hash_e);
    generateResponse();

    return _transcript;
}


bool SigmaProtocol::verifyNIZKP(const Transcript& nizkp)
{
    setTranscript(nizkp);

    if (verify() == false)
        return false;

    auto hash_e = GenHashChallenge(getHashData(), *_order);
    if (hash_e != _transcript.challenge())
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


// CompressedTranscript CompressTranscript(const Transcript& transcript)
// {
//     CompressedTranscript ret;
//     for (auto com : transcript.commitment) {
//         ret.commitment.push_back(CompressPoint(com));
//     }
//     ret.challenge = transcript.challenge;
//     ret.response = transcript.response;

//     return ret;
// }


// Transcript DecompressTranscript(const CompressedTranscript& compressed_transcript,
//                                 const CryptoPP::ECP& curve)
// {
//     Transcript ret;
//     for (auto com : compressed_transcript.commitment) {
//         ret.commitment.push_back(DecompressPoint(com, curve));
//     }
//     ret.challenge = compressed_transcript.challenge;
//     ret.response = compressed_transcript.response;

//     return ret;
// }