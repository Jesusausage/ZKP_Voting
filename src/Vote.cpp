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


Vote::Vote(Vote&& vote)
{
    swap(*this, vote);
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


Vote& Vote::operator=(Vote vote)
{
    swap(*this, vote);
    return *this;
}


void swap(Vote& a, Vote& b)
{
    std::swap(a._values, b._values);
    std::swap(a._proofs, b._proofs);
    std::swap(a._num_options, b._num_options);
}