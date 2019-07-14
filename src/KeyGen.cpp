#include "KeyGen.hpp"


KeyGen::KeyGen(const ECGroup& ecg,
               const CryptoPP::ECPPoint& generator,
               const std::vector<CryptoPP::ECPPoint>& token_sums,
               const CryptoPP::ECPPoint& id)
               :
               _ecg(&ecg),
               _gen(&generator),
               _token_sums(token_sums),
               _id(id)
{
    _num_options = _token_sums.size();

    _prot = new ElGamalProtocol(*_ecg, *_gen, 0);
}


KeyGen::~KeyGen()
{
    delete _prot;
}


void KeyGen::setIDKey(const CryptoPP::Integer& id_key)
{
    _id_key = id_key;
}


Key KeyGen::getKeysAndProofs()
{   
    _generateKeys();
    return {_keys, _proofs};
}


CompressedKey KeyGen::getCompressedKey()
{
    _generateKeys();
    
    CompressedKey ret;
    for (int i = 0; i < _num_options; i++) {
        ret.values.push_back(CompressPoint(_keys[i]));
        ret.proofs.push_back(CompressTranscript(_proofs[i]));
    }

    return ret;
}


void KeyGen::_generateKeys()
{
    for (int i = 0; i < _num_options; i++) {
        _keys.push_back(_ecg->curve.Multiply(_id_key, _token_sums[i]));

        _prot->setParams(_token_sums[i], _id, _keys[i], _id_key);
        _proofs.push_back(_prot->generateNIZKP());
    }
}