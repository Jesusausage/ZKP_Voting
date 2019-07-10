#include "SigmaProtocol.hpp"


CryptoPP::Integer GenHashChallenge(const std::string& hash_data,
                                   const CryptoPP::Integer& challenge_max)
{
    auto challenge_size = challenge_max.ByteCount();
    CryptoPP::byte* digest = new CryptoPP::byte[challenge_size];

    CryptoPP::SHA3_256 hash;
    hash.Update((CryptoPP::byte*)hash_data.data(), hash_data.size());
    hash.TruncatedFinal(digest, challenge_size);

    auto decoded_int = CryptoPP::Integer(digest, challenge_size);
    delete digest;

    return decoded_int % challenge_max;
}


Transcript SigmaProtocol::getTranscript()
{
    return {commitment(), challenge(), response()};
}


Transcript SigmaProtocol::generateNIZKP()
{
    generateCommitment();
    std::string hash_data = getHashData();
    auto hash_challenge = GenHashChallenge(hash_data, challengeSize());
    generateChallenge(&hash_challenge);
    generateResponse();
    assert(verify() == true);

    return {commitment(), challenge(), response()};
}


bool SigmaProtocol::verifyNIZKP(const Transcript& nizkp)
{
    if (verifyTranscript(nizkp) == false)
        return false;

    std::string hash_data = getHashData();
    auto hash_challenge = GenHashChallenge(hash_data, challengeSize());
    if (hash_challenge != challenge())
        return false;

    return true;
}