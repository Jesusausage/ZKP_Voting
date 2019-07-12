#ifndef VOTE_HPP
#define VOTE_HPP


#include "OrProtocol.hpp"
#include "ElGamalProtocol.hpp"


struct Vote {
    OrNIZKP proof;
    CryptoPP::ECPPoint vote;
};


class Voter {
public:
    Voter(const ECGroup& ecg,
          const CryptoPP::ECPPoint& generator,
          const CryptoPP::ECPPoint& id_sum,
          const std::vector<CryptoPP::ECPPoint>& tokens);
    void setTokenKeys(const std::vector<CryptoPP::Integer>& token_keys);
    void castVote(int option);
    std::vector<Vote> getVoteAndProofs();

private:
    const ECGroup* _ecg;
    const CryptoPP::ECPPoint _gen;
    const CryptoPP::ECPPoint _id_sum;
    const std::vector<CryptoPP::ECPPoint> _tokens;
    
    int _num_options;
    std::vector<CryptoPP::Integer> _token_keys;

    int _selected_option = -1;
    std::vector<CryptoPP::ECPPoint> _votes;

    OrNIZKP _generateProof(int option);
};


#endif