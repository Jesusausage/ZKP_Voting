#ifndef VOTER_HPP
#define VOTER_HPP


#include "OrProtocol.hpp"
#include "ElGamalProtocol.hpp"
#include "Vote.hpp"


class Voter {
public:
    Voter(const ECGroup& ecg,
          const CryptoPP::ECPPoint& generator,
          const CryptoPP::ECPPoint& id_sum,
          const std::vector<CryptoPP::ECPPoint>& tokens);
    ~Voter();
    void setTokenKeys(const std::vector<CryptoPP::Integer>& token_keys);
    void castVote(int option);
    Vote getVoteAndProofs();

private:
    const ECGroup* ecg_;
    const CryptoPP::ECPPoint* gen_;
    const CryptoPP::ECPPoint id_sum_;
    const std::vector<CryptoPP::ECPPoint> tokens_;
    
    int num_options_;
    std::vector<CryptoPP::Integer> token_keys_;

    Vote vote_;

    ElGamalProtocol* prots_[2];
    OrProtocol* or_prot_;
};


#endif