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
    bool verifyVoteProofs(const Vote& vote);
    void setID(const CryptoPP::ECPPoint& id);
    bool verifyKeyProofs(const Key& key);

private:
    const ECGroup* ecg_;
    const CryptoPP::ECPPoint gen_;
    const CryptoPP::ECPPoint id_sum_;
    const std::vector<CryptoPP::ECPPoint> token_sums_;

    std::vector<CryptoPP::ECPPoint> tokens_;
    CryptoPP::ECPPoint id_;

    ElGamalProtocol* vote_prots_[2];
    OrProtocol* vote_or_prot_;
    ElGamalProtocol* key_prot_;
};


#endif