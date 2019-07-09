#include "SigmaProtocol.hpp"


CryptoPP::Integer SigmaProtocol::_genHashChallenge(const std::string& hash_data)
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