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
    // CompressedVote getCompressedVote();

private:
    const ECGroup* _ecg;
    const CryptoPP::ECPPoint* _gen;
    const CryptoPP::ECPPoint _id_sum;
    const std::vector<CryptoPP::ECPPoint> _tokens;
    
    int _num_options;
    std::vector<CryptoPP::Integer> _token_keys;

    Vote _vote;

    ElGamalProtocol* _prots[2];
    OrProtocol* _or_prot;
};


#endif