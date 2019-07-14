#ifndef VOTE_HPP
#define VOTE_HPP


#include "OrProtocol.hpp"
#include "ElGamalProtocol.hpp"


struct Vote {
    std::vector<CryptoPP::ECPPoint> values;
    std::vector<OrNIZKP> proofs;
};


struct CompressedVote {
    std::vector<CompressedPoint> values;
    std::vector<CompressedOrNIZKP> proofs;
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
    CompressedVote getCompressedVote();

private:
    const ECGroup* _ecg;
    const CryptoPP::ECPPoint* _gen;
    const CryptoPP::ECPPoint _id_sum;
    const std::vector<CryptoPP::ECPPoint> _tokens;
    
    int _num_options;
    std::vector<CryptoPP::Integer> _token_keys;

    std::vector<CryptoPP::ECPPoint> _votes;
    std::vector<OrNIZKP> _proofs;

    ElGamalProtocol* _prots[2];
    OrProtocol* _or_prot;
};


#endif