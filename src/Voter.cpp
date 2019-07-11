#include "Voter.hpp"


Voter::Voter(const ECGroup& ecg,
             const CryptoPP::ECPPoint& generator,
             const IDInfo& id_info,
             const std::vector<VoteTokenInfo>& token_info)
             :
             _ecg(&ecg),
             _gen(generator),
             _id_info(id_info),
             _token_info(token_info)
{
    _num_options = _token_info.size();
}


void Voter::castVote(int option)
{
    _selected_option = option;

    auto identity = _ecg->curve.Identity();

    for (int i = 0; i < _num_options; i++) {
        auto a = (i == option) ? _gen : identity;
        auto b = _ecg->curve.Multiply(_token_info[i].token_key, 
                                      _id_info.id_sum);
        _votes.push_back(_ecg->curve.Add(a, b));
    }
}


Vote Voter::getVoteAndProofs()
{
    assert(_selected_option >= 0);
    
    std::vector<OrNIZKP> proofs;
    for (int i = 0; i < _num_options; i++)
        proofs.push_back(_generateProof(i));

    return {proofs, _votes};
}


OrNIZKP Voter::_generateProof(int option)
{
    auto gen2 = _id_info.id_sum;
    auto pub1 = _token_info[option].token;
    auto pub2 = _votes[option];
    auto witness = _token_info[option].token_key;

    if (option == _selected_option) {
        ElGamalProtocol prot0(*_ecg, _gen, gen2, pub1, pub2, 0);
        ElGamalProtocol prot1(*_ecg, _gen, gen2, pub1, pub2, 1, witness);
        OrProtocol prot({&prot0, &prot1}, 1);
        return prot.generateNIZKP();
    }
    else {
        ElGamalProtocol prot0(*_ecg, _gen, gen2, pub1, pub2, 0, witness);
        ElGamalProtocol prot1(*_ecg, _gen, gen2, pub1, pub2, 1);
        OrProtocol prot({&prot0, &prot1}, 0);
        return prot.generateNIZKP();
    }    
}