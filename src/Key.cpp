#include "Key.hpp"


Key::Key(CryptoPP::ECPPoint* values, Transcript* proofs, int num_options)
         : 
         _num_options(num_options)
{
    _values = new CryptoPP::ECPPoint[_num_options];
    _proofs = new Transcript[_num_options];
    for (int i = 0; i < _num_options; i++) {
        _values[i] = values[i];
        _proofs[i] = proofs[i];
    }
}


Key::Key(int num_options) : _num_options(num_options)
{
    _values = new CryptoPP::ECPPoint[_num_options];
    _proofs = new Transcript[_num_options];
}


Key::Key(const Key& key) : _num_options(key._num_options)
{
    _values = new CryptoPP::ECPPoint[_num_options];
    _proofs = new Transcript[_num_options];
    for (int i = 0; i < _num_options; i++) {
        _values[i] = key._values[i];
        _proofs[i] = key._proofs[i];
    }
}


Key::Key()
{}


Key::~Key()
{
    if (_values)
        delete [] _values;
    if (_proofs)
        delete [] _proofs;
}


Key& Key::operator=(const Key& key)
{
    if (this == &key)
        return *this;

    _num_options = key._num_options;

    if (_values)
        delete [] _values;
    if (_proofs)
        delete [] _proofs;
    _proofs = new Transcript[_num_options];
    _values = new CryptoPP::ECPPoint[_num_options];
    for (int i = 0; i < _num_options; i++) {
        _values[i] = key._values[i];
        _proofs[i] = key._proofs[i];
    }

    return *this;
}


void Key::setValue(int i, CryptoPP::ECPPoint value)
{
    _values[i] = value;
}


void Key::setProof(int i, Transcript proof)
{
    _proofs[i] = proof;
}


CryptoPP::ECPPoint Key::value(int i) const
{
    return _values[i];
}


Transcript Key::proof(int i) const
{
    return _proofs[i];
}


int Key::num_options() const
{
    return _num_options;
}