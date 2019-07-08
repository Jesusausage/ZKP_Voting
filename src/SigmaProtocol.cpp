#include "SigmaProtocol.hpp"


bool SigmaProtocol::produceNIZKP()
{
    generateCommitment();

    std::string hash_data = getHashData();
    auto challenge = genHashChallenge(hash_data);
    generateChallenge(&challenge);
    
    return (generateResponse() && verify());
}


CryptoPP::Integer SigmaProtocol::genHashChallenge(const std::string& hash_data)
{
    auto challenge_size = challengeSize().ByteCount();
    CryptoPP::byte* digest = new CryptoPP::byte[challenge_size];

    CryptoPP::SHA3_256 hash;
    hash.Update((CryptoPP::byte*)hash_data.data(), hash_data.size());
    hash.TruncatedFinal(digest, challenge_size);

    auto decoded_int = CryptoPP::Integer(digest, challenge_size);
    delete digest;

    return decoded_int % challengeSize();
}