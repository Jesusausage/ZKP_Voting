#ifndef VERIFIER_HPP
#define VERIFIER_HPP


#include "OrProtocol.hpp"
#include "ElGamalProtocol.hpp"
#include "Voter.hpp"
#include "KeyGen.hpp"


class Verifier {
public:
    Verifier(const ECGroup& ecg,
             const CryptoPP::ECPPoint& generator,
             const CryptoPP::ECPPoint& id_sum,
             const std::vector<CryptoPP::ECPPoint>& token_sums);
    ~Verifier();
    void setVoterTokens(const std::vector<CryptoPP::ECPPoint>& tokens);
    bool verifyVoteProofs(const Vote& votes);
    void setID(const CryptoPP::ECPPoint& id);
    bool verifyKeyProofs(const Key& keys);
    Vote decompressVote(const CompressedVote& compressed_vote);
    Key decompressKey(const CompressedKey& compressed_key);

private:
    const ECGroup* _ecg;
    const CryptoPP::ECPPoint _gen;
    const CryptoPP::ECPPoint _id_sum;
    const std::vector<CryptoPP::ECPPoint> _token_sums;

    std::vector<CryptoPP::ECPPoint> _tokens;
    CryptoPP::ECPPoint _id;

    ElGamalProtocol* _vote_prots[2];
    OrProtocol* _vote_prot;
    ElGamalProtocol* _key_prot;
};


#endif


// CREATE VOTE AND KEY PORTION COMPRESSION AND DECOMPRESSION