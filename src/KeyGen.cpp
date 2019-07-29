#include "KeyGen.hpp"


KeyGen::KeyGen(const ECGroup& ecg,
               const CryptoPP::ECPPoint& generator,
               const std::vector<CryptoPP::ECPPoint>& token_sums,
               const CryptoPP::ECPPoint& id)
               :
               ecg_(ecg),
               gen_(generator),
               id_(id),
               num_options_(token_sums.size()),
               key_(token_sums.size()),
               prot_(ecg, generator, 0)
{
    token_sums_ = new CryptoPP::ECPPoint[num_options_];
    for (int i = 0; i < num_options_; i++)
        token_sums_[i] = token_sums[i];
}


KeyGen::KeyGen(const ECGroup& ecg,
               const CryptoPP::ECPPoint& generator,
               const CryptoPP::ECPPoint token_sums[],
               const CryptoPP::ECPPoint& id,
               const int num_options)
               :
               ecg_(ecg),
               gen_(generator),
               id_(id),
               num_options_(num_options),
               key_(num_options),
               prot_(ecg, generator, 0)
{
    token_sums_ = new CryptoPP::ECPPoint[num_options_];
    for (int i = 0; i < num_options_; i++)
        token_sums_[i] = token_sums[i];
}


KeyGen::~KeyGen()
{
    delete [] token_sums_;
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
        key_.setValue(i, ecg_.curve.Multiply(id_key_, token_sums_[i]));

        prot_.setParams(token_sums_[i], id_, key_.value(i), id_key_);
        key_.setProof(i, prot_.generateNIZKP());
    }
}