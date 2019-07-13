#ifndef VOTE_HPP
#define VOTE_HPP


#include "OrProtocol.hpp"
#include "ElGamalProtocol.hpp"


struct Vote {
    std::vector<CryptoPP::ECPPoint> values;
    std::vector<OrNIZKP> proofs;
};


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
    const ECGroup* _ecg;
    const CryptoPP::ECPPoint* _gen;
    const CryptoPP::ECPPoint _id_sum;
    const std::vector<CryptoPP::ECPPoint> _tokens;
    
    int _num_options;
    std::vector<CryptoPP::Integer> _token_keys;

    int _selected_option = -1;
    std::vector<CryptoPP::ECPPoint> _votes;

    ElGamalProtocol* _prots[2];
    OrProtocol* _or_prot;

    OrNIZKP _generateProof(int option);
};


#endif