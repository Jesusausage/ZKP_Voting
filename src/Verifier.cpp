#include "Verifier.hpp"


Verifier::Verifier(const ECGroup& ecg,
                   const CryptoPP::ECPPoint& generator,
                   const CryptoPP::ECPPoint& id_sum,
                   const std::vector<CryptoPP::ECPPoint>& token_sums)
                   :
                   _ecg(&ecg),
                   _gen(generator),
                   _id_sum(id_sum),
                   _token_sums(token_sums)
{
    _vote_prots[0] = new ElGamalProtocol(*_ecg, _gen, 0);
    _vote_prots[1] = new ElGamalProtocol(*_ecg, _gen, 1);
    _vote_prot = new OrProtocol({_vote_prots[0], _vote_prots[1]});

    _key_prot = new ElGamalProtocol(*_ecg, _gen, 0);
}


Verifier::~Verifier()
{
    delete _vote_prots[0];
    delete _vote_prots[1];
    delete _vote_prot;

    delete _key_prot;
}


void Verifier::setVoterTokens(const std::vector<CryptoPP::ECPPoint>& tokens)
{
    _tokens = tokens;
}


void Verifier::setID(const CryptoPP::ECPPoint& id)
{
    _id = id;
}


bool Verifier::verifyVoteProofs(const Vote& vote)
{
    int num_options = vote.num_options();

    for (int i = 0; i < num_options; i++) {
        _vote_prots[0]->setParams(_id_sum, _tokens[i], vote.value(i));
        _vote_prots[1]->setParams(_id_sum, _tokens[i], vote.value(i));

        if (_vote_prot->verifyNIZKP(vote.proof(i)) == false)
            return false;
    }

    return true;
}


bool Verifier::verifyKeyProofs(const Key& key)
{
    int num_options = key.num_options();

    for (int i = 0; i < num_options; i++) {
        _key_prot->setParams(_token_sums[i], _id, key.value(i));
        if (_key_prot->verifyNIZKP(key.proof(i)) == false)
            return false;
    }

    return true;
}


// Vote Verifier::decompressVote(const CompressedVote& compressed_vote)
// {
//     Vote ret;
//     int num_options = compressed_vote.values.size();
//     for (int i = 0; i < num_options; i++) {
//         ret.values.push_back(DecompressPoint(compressed_vote.values[i], 
//                                              _ecg->curve));
//         ret.proofs.push_back(DecompressOrNIZKP(compressed_vote.proofs[i],
//                                                _ecg->curve));
//     }

//     return ret;
// }


// Key Verifier::decompressKey(const CompressedKey& compressed_key)
// {
//     Key ret;
//     int num_options = compressed_key.values.size();
//     for (int i = 0; i < num_options; i++) {
//         ret.values.push_back(DecompressPoint(compressed_key.values[i],
//                                              _ecg->curve));
//         ret.proofs.push_back(DecompressTranscript(compressed_key.proofs[i],
//                                                   _ecg->curve));
//     }

//     return ret;
// }