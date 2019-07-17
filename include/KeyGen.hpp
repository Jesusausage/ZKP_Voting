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
    const ECGroup* _ecg;
    const CryptoPP::ECPPoint* _gen;
    const std::vector<CryptoPP::ECPPoint> _token_sums;
    const CryptoPP::ECPPoint _id;
    
    int _num_options;
    CryptoPP::Integer _id_key;

    Key _key;

    ElGamalProtocol* _prot = nullptr;

    void _generateKeys();
};


#endif