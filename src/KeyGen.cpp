#include "KeyGen.hpp"


KeyGen::KeyGen(const ECGroup& ecg,
               const CryptoPP::ECPPoint& generator,
               const std::vector<CryptoPP::ECPPoint>& token_sums,
               const CryptoPP::ECPPoint& id)
               :
               ecg_(&ecg),
               gen_(&generator),
               token_sums_(token_sums),
               id_(id),
               num_options_(token_sums_.size()),
               key_(token_sums_.size())
{
    prot_ = new ElGamalProtocol(*ecg_, *gen_, 0);
}


KeyGen::~KeyGen()
{
    delete prot_;
}


void KeyGen::setIDKey(const CryptoPP::Integer& id_key)
{
    id_key_ = id_key;
}


Key KeyGen::getKeysAndProofs()
{   
    generateKeys();
    return key_;
}


void KeyGen::generateKeys()
{
    for (int i = 0; i < num_options_; i++) {
        key_.setValue(i, ecg_->curve.Multiply(id_key_, token_sums_[i]));

        prot_->setParams(token_sums_[i], id_, key_.value(i), id_key_);
        key_.setProof(i, prot_->generateNIZKP());
    }
}