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
    if (_r)
        delete [] _r;
    _r = new CryptoPP::ECPPoint[_r_size];
    for (int i = 0; i < _r_size; i++) {
        _r[i] = transcript.commitment()[i];
    }
}


Transcript::Transcript()
{}


Transcript::~Transcript()
{
    if (_r)
        delete [] _r;
}


void Transcript::setCommitment(CryptoPP::ECPPoint* commitment, 
                               int commitment_size)
{
    if (_r)
        delete [] _r;
    _r_size = commitment_size;
    _r = new CryptoPP::ECPPoint[_r_size];
    for (int i = 0; i < _r_size; i++) {
        _r[i] = commitment[i];
    }
}


void Transcript::setChallenge(const CryptoPP::Integer& challenge)
{
    _e = challenge;
}


void Transcript::setResponse(const CryptoPP::Integer& response)
{
    _s = response;
}


CryptoPP::ECPPoint* Transcript::commitment() const
{
    return _r;
}


int Transcript::commitmentSize() const
{
    return _r_size;
}


CryptoPP::Integer Transcript::challenge() const
{
    return _e;
}


CryptoPP::Integer Transcript::response() const
{
    return _s;
}