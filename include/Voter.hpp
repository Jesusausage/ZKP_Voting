#ifndef VOTE_HPP
#define VOTE_HPP


#include "OrProtocol.hpp"
#include "ElGamalProtocol.hpp"


struct Vote {
    std::vector<OrNIZKP> proofs;
    std::vector<CryptoPP::ECPPoint> votes;
};


struct IDInfo {
    CryptoPP::ECPPoint id;
    CryptoPP::ECPPoint id_sum;
    CryptoPP::Integer id_key;
};


struct VoteTokenInfo {
    CryptoPP::ECPPoint token;
    CryptoPP::ECPPoint token_sum;
    CryptoPP::Integer token_key;
};


class Voter {
public:
    Voter(const ECGroup& ecg,
          const CryptoPP::ECPPoint& generator,
          const IDInfo& id_info,
          const std::vector<VoteTokenInfo>& token_info);
    void castVote(int option);
    Vote getVoteAndProofs();

private:
    const ECGroup* _ecg;
    const CryptoPP::ECPPoint _gen;
    IDInfo _id_info;
    std::vector<VoteTokenInfo> _token_info;

    int _num_options;
    int _selected_option = -1;
    std::vector<CryptoPP::ECPPoint> _votes;

    OrNIZKP _generateProof(int option);
};


#endif