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


Key::Key(Key&& key)
{
    swap(*this, key);
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


Key& Key::operator=(Key key)
{
    swap(*this, key);
    return *this;
}


void swap(Key& a, Key& b)
{
    std::swap(a._values, b._values);
    std::swap(a._proofs, b._proofs);
    std::swap(a._num_options, b._num_options);
}