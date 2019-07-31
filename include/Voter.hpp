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

    inline void setTokenKeys(const std::vector<CryptoPP::Integer>& token_keys)
        { token_keys_ = token_keys; }
    inline void setTokenKeys(const CryptoPP::Integer token_keys[])
        { token_keys_ = std::vector<CryptoPP::Integer>(token_keys, token_keys+num_options_); }
    void castVote(int option);
    inline const Vote& getVoteAndProofs() const
        { return vote_; }

private:
    const ECGroup& ecg_;
    const CryptoPP::ECPPoint& gen_;
    const CryptoPP::ECPPoint& id_sum_;
    const std::vector<CryptoPP::ECPPoint> tokens_;    
    const int num_options_;

    std::vector<CryptoPP::Integer> token_keys_;

    ElGamalProtocol* prots_[2];
    OrProtocol* or_prot_;

    Vote vote_;
};


#endif