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

    std::vector<Transcript> proofs;
    for (int i = 0; i < _num_options; i++)
        proofs.push_back(_generateProof(i));

    return {_keys, proofs};
}


void KeyGen::_generateKeys()
{
    for (int i = 0; i < _num_options; i++)
        _keys.push_back(_ecg->curve.Multiply(_id_key, _token_sums[i]));
}


Transcript KeyGen::_generateProof(int option)
{
    _prot->setParams(_token_sums[option], _id, _keys[option], _id_key);
    return _prot->generateNIZKP();
}