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
    ~KeyGen();

    void setIDKey(const CryptoPP::Integer& id_key);
    Key getKeysAndProofs();

private:
    const ECGroup* ecg_;
    const CryptoPP::ECPPoint* gen_;
    const std::vector<CryptoPP::ECPPoint> token_sums_;
    const CryptoPP::ECPPoint id_;
    
    int num_options_;
    CryptoPP::Integer id_key_;

    Key key_;

    ElGamalProtocol* prot_ = nullptr;

    void generateKeys();
};


#endif