#include "Voter.hpp"


Voter::Voter(const ECGroup& ecg,
             const CryptoPP::ECPPoint& generator,
             const CryptoPP::ECPPoint& id_sum,
             const std::vector<CryptoPP::ECPPoint>& tokens)
             :
             _ecg(&ecg),
             _gen(&generator),
             _id_sum(id_sum),
             _tokens(tokens)
{
    _num_options = _tokens.size();

    _prots[0] = new ElGamalProtocol(*_ecg, *_gen, _id_sum, 0);
    _prots[1] = new ElGamalProtocol(*_ecg, *_gen, _id_sum, 1);
    _or_prot = new OrProtocol({_prots[0], _prots[1]});
}


Voter::~Voter()
{
    delete _prots[0];
    delete _prots[1];
    delete _or_prot;
}


void Voter::setTokenKeys(const std::vector<CryptoPP::Integer>& token_keys)
{
    assert(token_keys.size() == (unsigned int)_num_options);
    _token_keys = token_keys;
}


void Voter::castVote(int option)
{
    _selected_option = option;

    auto identity = _ecg->curve.Identity();

    for (int i = 0; i < _num_options; i++) {
        auto a = (i == option) ? *_gen : identity;
        auto b = _ecg->curve.Multiply(_token_keys[i], 
                                      _id_sum);
        _votes.push_back(_ecg->curve.Add(a, b));
    }
}


Vote Voter::getVoteAndProofs()
{
    assert(_selected_option >= 0);
    
    std::vector<OrNIZKP> proofs;
    for (int i = 0; i < _num_options; i++)
        proofs.push_back(_generateProof(i));

    return {_votes, proofs};
}


OrNIZKP Voter::_generateProof(int option)
{
    int known = (option == _selected_option) ? 1 : 0;

    _prots[known]->setKeys(_tokens[option], _votes[option], 
                           _token_keys[option]);
    _prots[1 - known]->setKeys(_tokens[option], _votes[option]);
    _or_prot->setKnown(known);

    return _or_prot->generateNIZKP();
}