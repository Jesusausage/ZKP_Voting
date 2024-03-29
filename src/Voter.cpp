#include "Voter.hpp"


Voter::Voter(const ECGroup& ecg,
             const CryptoPP::ECPPoint& generator,
             const CryptoPP::ECPPoint& id_sum,
             const std::vector<CryptoPP::ECPPoint>& tokens)
             :
             ecg_(ecg),
             gen_(generator),
             id_sum_(id_sum),
             tokens_(tokens),
             num_options_(tokens.size()),
             vote_(tokens.size())
{
    prots_[0] = new ElGamalProtocol(ecg_, gen_, 0);
    prots_[1] = new ElGamalProtocol(ecg_, gen_, 1);
    or_prot_ = new OrProtocol((SigmaProtocol**)prots_, 2);
}


Voter::Voter(const ECGroup& ecg,
             const CryptoPP::ECPPoint& generator,
             const CryptoPP::ECPPoint& id_sum,
             const CryptoPP::ECPPoint tokens[],
             const int num_options)
             :
             ecg_(ecg),
             gen_(generator),
             id_sum_(id_sum),
             tokens_(tokens, tokens + num_options),
             num_options_(num_options),
             vote_(num_options)
{
    prots_[0] = new ElGamalProtocol(ecg_, gen_, 0);
    prots_[1] = new ElGamalProtocol(ecg_, gen_, 1);
    or_prot_ = new OrProtocol((SigmaProtocol**)prots_, 2);
}


Voter::~Voter()
{
    delete prots_[0];
    delete prots_[1];
    delete or_prot_;
}


void Voter::castVote(int option)
{
    auto identity = ecg_.curve.Identity();

    for (int i = 0; i < num_options_; i++) {
        auto a = (i == option) ? gen_ : identity;
        auto b = ecg_.curve.Multiply(token_keys_[i], id_sum_);
        vote_.setValue(i, ecg_.curve.Add(a, b));

        int known = (i == option) ? 1 : 0;
        prots_[known]->setParams(id_sum_, tokens_[i], vote_.value(i), token_keys_[i]);
        prots_[1 - known]->setParams(id_sum_, tokens_[i], vote_.value(i));
        or_prot_->setKnown(known);
        vote_.setProof(i, or_prot_->generateNIZKP());
    }
}