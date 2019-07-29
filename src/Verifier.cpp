#include "Verifier.hpp"


Verifier::Verifier(const ECGroup& ecg,
                   const CryptoPP::ECPPoint& generator,
                   const CryptoPP::ECPPoint& id_sum,
                   const std::vector<CryptoPP::ECPPoint>& token_sums)
                   :
                   ecg_(&ecg),
                   gen_(generator),
                   num_options_(token_sums.size()),
                   id_sum_(id_sum)
{
    vote_prots_[0] = new ElGamalProtocol(*ecg_, gen_, 0);
    vote_prots_[1] = new ElGamalProtocol(*ecg_, gen_, 1);
    vote_or_prot_ = new OrProtocol((SigmaProtocol**)vote_prots_, 2);

    key_prot_ = new ElGamalProtocol(*ecg_, gen_, 0);

    token_sums_ = new CryptoPP::ECPPoint[num_options_];
    for (int i = 0; i < num_options_; i++)
        token_sums_[i] = token_sums[i];
        
    tokens_ = new CryptoPP::ECPPoint[num_options_];
}


Verifier::Verifier(const ECGroup& ecg,
                   const CryptoPP::ECPPoint& generator,
                   const CryptoPP::ECPPoint& id_sum,
                   const CryptoPP::ECPPoint token_sums[],
                   const int num_options)
                   :
                   ecg_(&ecg),
                   gen_(generator),
                   num_options_(num_options),
                   id_sum_(id_sum)
{
    vote_prots_[0] = new ElGamalProtocol(*ecg_, gen_, 0);
    vote_prots_[1] = new ElGamalProtocol(*ecg_, gen_, 1);
    vote_or_prot_ = new OrProtocol((SigmaProtocol**)vote_prots_, 2);

    key_prot_ = new ElGamalProtocol(*ecg_, gen_, 0);

    token_sums_ = new CryptoPP::ECPPoint[num_options_];
    for (int i = 0; i < num_options_; i++)
        token_sums_[i] = token_sums[i];
        
    tokens_ = new CryptoPP::ECPPoint[num_options_];
}


Verifier::~Verifier()
{
    delete vote_prots_[0];
    delete vote_prots_[1];
    delete vote_or_prot_;

    delete key_prot_;

    delete [] token_sums_;
    delete [] tokens_;
}


void Verifier::setTokens(const std::vector<CryptoPP::ECPPoint>& tokens)
{
    for (int i = 0; i < num_options_; i++)
        tokens_[i] = tokens[i];
}


void Verifier::setTokens(const CryptoPP::ECPPoint tokens[])
{
    for (int i = 0; i < num_options_; i++)
        tokens_[i] = tokens[i];
}


void Verifier::setID(const CryptoPP::ECPPoint& id)
{
    id_ = id;
}


bool Verifier::verifyVote(const Vote& vote)
{
    for (int i = 0; i < num_options_; i++) {
        vote_prots_[0]->setParams(id_sum_, tokens_[i], vote.value(i));
        vote_prots_[1]->setParams(id_sum_, tokens_[i], vote.value(i));
        if (vote_or_prot_->verifyNIZKP(vote.proof(i)) == false)
            return false;
    }

    return true;
}


bool Verifier::verifyKey(const Key& key)
{
    for (int i = 0; i < num_options_; i++) {
        key_prot_->setParams(token_sums_[i], id_, key.value(i));
        if (key_prot_->verifyNIZKP(key.proof(i)) == false)
            return false;
    }

    return true;
}