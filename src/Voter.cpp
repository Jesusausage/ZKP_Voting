#include "Voter.hpp"


Voter::Voter(const ECGroup& ecg,
             const CryptoPP::ECPPoint& generator,
             const CryptoPP::ECPPoint& id_sum,
             const std::vector<CryptoPP::ECPPoint>& tokens)
             :
             _ecg(&ecg),
             _gen(&generator),
             _id_sum(id_sum),
             _tokens(tokens),
             _num_options(tokens.size()),
             _vote(tokens.size())
{
    _prots[0] = new ElGamalProtocol(*_ecg, *_gen, 0);
    _prots[1] = new ElGamalProtocol(*_ecg, *_gen, 1);
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
    auto identity = _ecg->curve.Identity();

    for (int i = 0; i < _num_options; i++) {
        auto a = (i == option) ? *_gen : identity;
        auto b = _ecg->curve.Multiply(_token_keys[i], _id_sum);
        _vote.setValue(i, _ecg->curve.Add(a, b));

        int known = (i == option) ? 1 : 0;
        _prots[known]->setParams(_id_sum, _tokens[i], _vote.value(i), _token_keys[i]);
        _prots[1 - known]->setParams(_id_sum, _tokens[i], _vote.value(i));
        _or_prot->setKnown(known);
        _vote.setProof(i, _or_prot->generateNIZKP());
    }
}


Vote Voter::getVoteAndProofs()
{
    return _vote;
}


// CompressedVote Voter::getCompressedVote()
// {
//     CompressedVote ret;
//     for (int i = 0; i < _num_options; i++) {
//         ret.values.push_back(CompressPoint(_votes[i]));
//         ret.proofs.push_back(CompressOrNIZKP(_proofs[i]));
//     }

//     return ret;
// }