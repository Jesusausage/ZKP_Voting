#include "Verifier.hpp"


Verifier::Verifier(const ECGroup& ecg,
                   const CryptoPP::ECPPoint& generator,
                   const CryptoPP::ECPPoint& id_sum,
                   const std::vector<CryptoPP::ECPPoint>& token_sums)
                   :
                   ecg_(&ecg),
                   gen_(generator),
                   id_sum_(id_sum),
                   token_sums_(token_sums)
{
    vote_prots_[0] = new ElGamalProtocol(*ecg_, gen_, 0);
    vote_prots_[1] = new ElGamalProtocol(*ecg_, gen_, 1);
    vote_or_prot_ = new OrProtocol({vote_prots_[0], vote_prots_[1]});

    key_prot_ = new ElGamalProtocol(*ecg_, gen_, 0);
}


Verifier::~Verifier()
{
    delete vote_prots_[0];
    delete vote_prots_[1];
    delete vote_or_prot_;

    delete key_prot_;
}


void Verifier::setVoterTokens(const std::vector<CryptoPP::ECPPoint>& tokens)
{
    tokens_ = tokens;
}


void Verifier::setID(const CryptoPP::ECPPoint& id)
{
    id_ = id;
}


bool Verifier::verifyVoteProofs(const Vote& vote)
{
    int num_options = vote.numOptions();

    for (int i = 0; i < num_options; i++) {
        vote_prots_[0]->setParams(id_sum_, tokens_[i], vote.value(i));
        vote_prots_[1]->setParams(id_sum_, tokens_[i], vote.value(i));

        if (vote_or_prot_->verifyNIZKP(vote.proof(i)) == false)
            return false;
    }

    return true;
}


bool Verifier::verifyKeyProofs(const Key& key)
{
    int num_options = key.numOptions();

    for (int i = 0; i < num_options; i++) {
        key_prot_->setParams(token_sums_[i], id_, key.value(i));
        if (key_prot_->verifyNIZKP(key.proof(i)) == false)
            return false;
    }

    return true;
}