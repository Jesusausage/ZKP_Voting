#include "Transcript.hpp"


Transcript::Transcript(CryptoPP::ECPPoint* commitment,
                       int commitment_size,
                       const CryptoPP::Integer& challenge,
                       const CryptoPP::Integer& response)
                       :
                       r_size_(commitment_size),
                       e_(challenge), 
                       s_(response) 
{
    r_ = new CryptoPP::ECPPoint[r_size_];
    for (int i = 0; i < r_size_; i++) {
        r_[i] = commitment[i];
    }
}


Transcript::Transcript(const Transcript& transcript)
                       :
                       r_size_(transcript.r_size_),
                       e_(transcript.e_), 
                       s_(transcript.s_)
{
    r_ = new CryptoPP::ECPPoint[r_size_];
    for (int i = 0; i < r_size_; i++) {
        r_[i] = transcript.r_[i];
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
    if (r_)
        delete [] r_;
}


Transcript& Transcript::operator=(Transcript transcript)
{
    swap(*this, transcript);
    return *this;
}


void swap(Transcript& a, Transcript& b)
{
    std::swap(a.r_, b.r_);
    std::swap(a.r_size_, b.r_size_);
    std::swap(a.e_, b.e_);
    std::swap(a.s_, b.s_);
}


void Transcript::setCommitment(CryptoPP::ECPPoint* commitment, 
                               int commitment_size)
{
    if (r_)
        delete [] r_;
    r_size_ = commitment_size;
    r_ = new CryptoPP::ECPPoint[r_size_];
    for (int i = 0; i < r_size_; i++) {
        r_[i] = commitment[i];
    }
}