#include "Transcript.hpp"


Transcript::Transcript(CryptoPP::ECPPoint* commitment,
                       int commitment_size,
                       const CryptoPP::Integer& challenge,
                       const CryptoPP::Integer& response)
                       :
                       _r_size(commitment_size),
                       _e(challenge), 
                       _s(response) 
{
    _r = new CryptoPP::ECPPoint[_r_size];
    for (int i = 0; i < _r_size; i++) {
        _r[i] = commitment[i];
    }
}


Transcript::Transcript(const Transcript& transcript)
                       :
                       _r_size(transcript._r_size),
                       _e(transcript._e), 
                       _s(transcript._s)
{
    _r = new CryptoPP::ECPPoint[_r_size];
    for (int i = 0; i < _r_size; i++) {
        _r[i] = transcript._r[i];
    }
}


Transcript::Transcript(Transcript&& transcript)
{
    swap(*this, transcript);
}


Transcript::Transcript()
{}


Transcript::~Transcript()
{
    if (_r)
        delete [] _r;
}


Transcript& Transcript::operator=(Transcript transcript)
{
    swap(*this, transcript);
    return *this;
}


void swap(Transcript& a, Transcript& b)
{
    std::swap(a._r, b._r);
    std::swap(a._r_size, b._r_size);
    std::swap(a._e, b._e);
    std::swap(a._s, b._s);
}


void Transcript::setCommitment(CryptoPP::ECPPoint* commitment, 
                               int commitment_size)
{
    if (_r == commitment)
        return;
    if (_r)
        delete [] _r;
    _r_size = commitment_size;
    _r = new CryptoPP::ECPPoint[_r_size];
    for (int i = 0; i < _r_size; i++) {
        _r[i] = commitment[i];
    }
}