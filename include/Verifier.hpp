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
    Verifier(const ECGroup& ecg,
             const CryptoPP::ECPPoint& generator,
             const CryptoPP::ECPPoint& id_sum,
             const CryptoPP::ECPPoint token_sums[],
             const int num_options);
    ~Verifier();
    
    void setTokens(const std::vector<CryptoPP::ECPPoint>& tokens);
    void setTokens(const CryptoPP::ECPPoint tokens[]);
    bool verifyVote(const Vote& vote);

    void setID(const CryptoPP::ECPPoint& id);
    bool verifyKey(const Key& key);

private:
    const ECGroup& ecg_;
    const CryptoPP::ECPPoint& gen_;
    const int num_options_;
    
    const CryptoPP::ECPPoint& id_sum_;
    CryptoPP::ECPPoint* tokens_;

    CryptoPP::ECPPoint* token_sums_;
    CryptoPP::ECPPoint id_;

    ElGamalProtocol* vote_prots_[2];
    OrProtocol* vote_or_prot_;
    ElGamalProtocol key_prot_;
};


#endif