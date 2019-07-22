#include "Voter.hpp"


Voter::Voter(const ECGroup& ecg,
             const CryptoPP::ECPPoint& generator,
             const CryptoPP::ECPPoint& id_sum,
             const std::vector<CryptoPP::ECPPoint>& tokens)
             :
             ecg_(&ecg),
             gen_(&generator),
             id_sum_(id_sum),
             num_options_(tokens.size()),
             vote_(tokens.size())
{
    prots_[0] = new ElGamalProtocol(*ecg_, *gen_, 0);
    prots_[1] = new ElGamalProtocol(*ecg_, *gen_, 1);
    or_prot_ = new OrProtocol({prots_[0], prots_[1]});

    tokens_ = new CryptoPP::ECPPoint[num_options_];
    for (int i = 0; i < num_options_; i++)
        tokens_[i] = tokens[i];

    token_keys_ = new CryptoPP::Integer[num_options_];
}


Voter::Voter(const ECGroup& ecg,
             const CryptoPP::ECPPoint& generator,
             const CryptoPP::ECPPoint& id_sum,
             const CryptoPP::ECPPoint tokens[],
             const int num_options)
             :
             ecg_(&ecg),
             gen_(&generator),
             id_sum_(id_sum),
             num_options_(num_options),
             vote_(num_options)
{
    prots_[0] = new ElGamalProtocol(*ecg_, *gen_, 0);
    prots_[1] = new ElGamalProtocol(*ecg_, *gen_, 1);
    or_prot_ = new OrProtocol({prots_[0], prots_[1]});

    tokens_ = new CryptoPP::ECPPoint[num_options_];
    for (int i = 0; i < num_options_; i++)
        tokens_[i] = tokens[i];

    token_keys_ = new CryptoPP::Integer[num_options_];
}


Voter::~Voter()
{
    delete prots_[0];
    delete prots_[1];
    delete or_prot_;

    delete [] tokens_;
    delete [] token_keys_;
}


void Voter::setTokenKeys(const std::vector<CryptoPP::Integer>& token_keys)
{
    for (int i = 0; i < num_options_; i++)
        token_keys_[i] = token_keys[i];
}


void Voter::setTokenKeys(const CryptoPP::Integer token_keys[])
{
    for (int i = 0; i < num_options_; i++)
        token_keys_[i] = token_keys[i];
}


void Voter::castVote(int option)
{
    auto identity = ecg_->curve.Identity();

    for (int i = 0; i < num_options_; i++) {
        auto a = (i == option) ? *gen_ : identity;
        auto b = ecg_->curve.Multiply(token_keys_[i], id_sum_);
        vote_.setValue(i, ecg_->curve.Add(a, b));

        int known = (i == option) ? 1 : 0;
        prots_[known]->setParams(id_sum_, tokens_[i], vote_.value(i), token_keys_[i]);
        prots_[1 - known]->setParams(id_sum_, tokens_[i], vote_.value(i));
        or_prot_->setKnown(known);
        vote_.setProof(i, or_prot_->generateNIZKP());
    }
}


Vote Voter::getVoteAndProofs()
{
    return vote_;
}