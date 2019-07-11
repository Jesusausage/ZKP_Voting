#include "Voter.hpp"


Voter::Voter(const ECGroup& ecg,
             const CryptoPP::ECPPoint& pub_id,
             const CryptoPP::Integer& priv_id_key,
             const std::vector<CryptoPP::ECPPoint>& pub_vote_tokens,
             const std::vector<CryptoPP::Integer>& priv_token_keys)
             :
             _curve(&ecg.curve),
             _id(pub_id),
             _id_key(priv_id_key),
             _tokens(pub_vote_tokens),
             _token_keys(priv_token_keys)
{
    _num_options = _tokens.size();
}


void Voter::castVote()
{

}


Vote Voter::getVoteAndProofs()
{
    assert(_selected_option >= 0);
    std::vector<OrNIZKP> proofs;

    for (int i = 0; i < _num_options; i++) {
        if (i == _selected_option) 
            proofs.push_back(_generateProof(i, true));
        else
            proofs.push_back(_generateProof(i, false));
    }

    return {proofs, _vote};
}


OrNIZKP Voter::_generateProof(int option, bool vote)
{
    
}