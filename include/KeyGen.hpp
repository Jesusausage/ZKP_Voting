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
    ~KeyGen();

    void setIDKey(const CryptoPP::Integer& id_key);
    Key getKeysAndProofs();

private:
    const ECGroup& ecg_;
    const CryptoPP::ECPPoint& gen_;
    const CryptoPP::ECPPoint& id_;
    CryptoPP::ECPPoint* token_sums_;
    const int num_options_;

    ElGamalProtocol prot_;
    CryptoPP::Integer id_key_;

    Key key_;

    void generateKeys();
};


#endif