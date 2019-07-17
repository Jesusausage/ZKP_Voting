#include "Vote.hpp"


Vote::Vote(CryptoPP::ECPPoint* values,
           OrTranscript* proofs,
           int num_options)
           :
           _num_options(num_options)
{
    _values = new CryptoPP::ECPPoint[_num_options];
    _proofs = new OrTranscript[_num_options];
    for (int i = 0; i < _num_options; i++) {
        _values[i] = values[i];
        _proofs[i] = proofs[i];
    }
}


Vote::Vote(int num_options) : _num_options(num_options)
{
    _values = new CryptoPP::ECPPoint[_num_options];
    _proofs = new OrTranscript[_num_options];
}


Vote::Vote(const Vote& vote) : _num_options(vote._num_options)
{
    _values = new CryptoPP::ECPPoint[_num_options];
    _proofs = new OrTranscript[_num_options];
    for (int i = 0; i < _num_options; i++) {
        _values[i] = vote._values[i];
        _proofs[i] = vote._proofs[i];
    }
}


Vote::Vote()
{}


Vote::~Vote()
{
    if (_values)
        delete [] _values;
    if (_proofs)
        delete [] _proofs;
}


Vote& Vote::operator=(const Vote& vote)
{
    if (this == &vote)
        return *this;

    _num_options = vote._num_options;
    if (_values)
        delete [] _values;
    if (_proofs)
        delete [] _proofs;
    _proofs = new OrTranscript[_num_options];
    _values = new CryptoPP::ECPPoint[_num_options];
    for (int i = 0; i < _num_options; i++) {
        _values[i] = vote._values[i];
        _proofs[i] = vote._proofs[i];
    }

    return *this;
}


void Vote::setValue(int i, CryptoPP::ECPPoint value)
{
    _values[i] = value;
}


void Vote::setProof(int i, OrTranscript proof)
{
    _proofs[i] = proof;
}


CryptoPP::ECPPoint Vote::value(int i) const
{
    return _values[i];
}


OrTranscript Vote::proof(int i) const
{
    return _proofs[i];
}


int Vote::num_options() const
{
    return _num_options;
}