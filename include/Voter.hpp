#ifndef VOTE_HPP
#define VOTE_HPP


#include "OrProtocol.hpp"
#include "ElGamalProtocol.hpp"


struct Vote {
    std::vector<OrNIZKP> proofs;
    CryptoPP::ECPPoint vote;
};


class Voter {
public:
    Voter(const ECGroup& ecg,
          const CryptoPP::ECPPoint& pub_id,
          const CryptoPP::Integer& priv_id_key,
          const std::vector<CryptoPP::ECPPoint>& pub_vote_tokens,
          const std::vector<CryptoPP::Integer>& priv_vote_keys);
    void castVote();
    Vote getVoteAndProofs();

private:
    const CryptoPP::ECP* _curve;

    CryptoPP::ECPPoint _id;
    CryptoPP::Integer _id_key;

    std::vector<CryptoPP::ECPPoint> _tokens;
    std::vector<CryptoPP::Integer> _token_keys;

    int _num_options;
    int _selected_option = -1;
    CryptoPP::ECPPoint _vote;
    
    OrNIZKP _generateProof(int option, bool vote);
};


#endif