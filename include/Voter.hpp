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
    Voter(const ECGroup& ecg,
          const CryptoPP::ECPPoint& generator,
          const CryptoPP::ECPPoint& id_sum,
          const CryptoPP::ECPPoint tokens[],
          const int num_options);
    ~Voter();

    void setTokenKeys(const std::vector<CryptoPP::Integer>& token_keys);
    void setTokenKeys(const CryptoPP::Integer token_keys[]);
    void castVote(int option);
    Vote getVoteAndProofs();

private:
    const ECGroup& ecg_;
    const CryptoPP::ECPPoint& gen_;
    const CryptoPP::ECPPoint& id_sum_;
    CryptoPP::ECPPoint* tokens_;    
    const int num_options_;

    ElGamalProtocol* prots_[2];
    OrProtocol* or_prot_;
    CryptoPP::Integer* token_keys_;

    Vote vote_;
};


#endif