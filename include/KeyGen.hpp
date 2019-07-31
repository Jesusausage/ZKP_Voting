#ifndef KEY_GEN_HPP
#define KEY_GEN_HPP


#include "ElGamalProtocol.hpp"
#include "Key.hpp"


class KeyGen {
public:
    KeyGen(const ECGroup& ecg,
           const CryptoPP::ECPPoint& generator,
           const std::vector<CryptoPP::ECPPoint>& token_sums,
           const CryptoPP::ECPPoint& id);
    KeyGen(const ECGroup& ecg,
           const CryptoPP::ECPPoint& generator,
           const CryptoPP::ECPPoint token_sums[],
           const CryptoPP::ECPPoint& id,
           const int num_options);

    inline void setIDKey(const CryptoPP::Integer& id_key)
        { id_key_ = id_key; }
    const Key& getKeysAndProofs();

private:
    const ECGroup& ecg_;
    const CryptoPP::ECPPoint& gen_;
    std::vector<CryptoPP::ECPPoint> token_sums_;
    const CryptoPP::ECPPoint& id_;
    const int num_options_;

    CryptoPP::Integer id_key_;

    ElGamalProtocol prot_;

    Key key_;

    void generateKeys();
};


#endif