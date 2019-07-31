#include "KeyGen.hpp"


KeyGen::KeyGen(const ECGroup& ecg,
               const CryptoPP::ECPPoint& generator,
               const std::vector<CryptoPP::ECPPoint>& token_sums,
               const CryptoPP::ECPPoint& id)
               :
               ecg_(ecg),
               gen_(generator),
               token_sums_(token_sums),
               id_(id),
               num_options_(token_sums.size()),
               prot_(ecg, generator, 0),
               key_(token_sums.size())
{}


KeyGen::KeyGen(const ECGroup& ecg,
               const CryptoPP::ECPPoint& generator,
               const CryptoPP::ECPPoint token_sums[],
               const CryptoPP::ECPPoint& id,
               const int num_options)
               :
               ecg_(ecg),
               gen_(generator),
               token_sums_(token_sums, token_sums + num_options),
               id_(id),
               num_options_(num_options),
               prot_(ecg, generator, 0),
               key_(num_options)
{}


const Key& KeyGen::getKeysAndProofs()
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