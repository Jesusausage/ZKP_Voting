#include "Voter.hpp"


Voter::Voter(const ECGroup& ecg,
             const CryptoPP::ECPPoint& generator,
             const CryptoPP::ECPPoint& id_sum,
             const std::vector<CryptoPP::ECPPoint>& tokens)
             :
             _ecg(&ecg),
             _gen(generator),
             _id_sum(id_sum),
             _tokens(tokens)
{
    _num_options = _tokens.size();
}


void Voter::setTokenKeys(const std::vector<CryptoPP::Integer>& token_keys)
{
    assert(token_keys.size() == _num_options);
    _token_keys = token_keys;
}


void Voter::castVote(int option)
{
    _selected_option = option;

    auto identity = _ecg->curve.Identity();

    for (int i = 0; i < _num_options; i++) {
        auto a = (i == option) ? _gen : identity;
        auto b = _ecg->curve.Multiply(_token_keys[i], 
                                      _id_sum);
        _votes.push_back(_ecg->curve.Add(a, b));
    }
}


std::vector<Vote> Voter::getVoteAndProofs()
{
    assert(_selected_option >= 0);
    
    std::vector<Vote> votes;
    for (int i = 0; i < _num_options; i++)
        votes.push_back({_generateProof(i), _votes[i]});

    return votes;
}


OrNIZKP Voter::_generateProof(int option)
{
    auto gen2 = _id_sum;
    auto pub1 = _tokens[option];
    auto pub2 = _votes[option];
    auto witness = _token_keys[option];

    if (option == _selected_option) {
        ElGamalProtocol prot0(*_ecg, _gen, gen2, 0, pub1, pub2);
        ElGamalProtocol prot1(*_ecg, _gen, gen2, 1, pub1, pub2, witness);
        OrProtocol prot({&prot0, &prot1}, 1);
        return prot.generateNIZKP();
    }
    else {
        ElGamalProtocol prot0(*_ecg, _gen, gen2, 0, pub1, pub2, witness);
        ElGamalProtocol prot1(*_ecg, _gen, gen2, 1, pub1, pub2);
        OrProtocol prot({&prot0, &prot1}, 0);
        return prot.generateNIZKP();
    }    
}