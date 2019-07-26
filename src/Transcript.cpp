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


void Transcript::serialise(CryptoPP::byte* output, 
                           int* commitment_size /*= nullptr*/)
{
    if (commitment_size)
        *commitment_size = r_size_;

    CompressedPoint p;
    int offset = 0;

    for (int i = 0; i < r_size_; i++) {
        p = CompressPoint(r_[i]);
        p.x.Encode(output+offset, FIELD_SIZE);
        offset += FIELD_SIZE;
        output[offset++] = (p.y == true) ? 1 : 0;
    }

    e_.Encode(output+offset, FIELD_SIZE);
    offset += FIELD_SIZE;

    s_.Encode(output+offset, FIELD_SIZE);
}


Transcript::Transcript(CryptoPP::byte* input, 
                       int commitment_size,
                       const CryptoPP::ECP& ec) 
                       : 
                       r_size_(commitment_size)
{
    r_ = new CryptoPP::ECPPoint[r_size_];
    CompressedPoint p;
    int offset = 0;

    for (int i = 0; i < r_size_; i++) {
        p.x = CryptoPP::Integer(input+offset, FIELD_SIZE);
        offset += FIELD_SIZE;
        p.y = (input[offset++] == 1) ? true : false;
        r_[i] = DecompressPoint(p, ec);
    }

    e_ = CryptoPP::Integer(input+offset, FIELD_SIZE);
    offset += FIELD_SIZE;

    s_ = CryptoPP::Integer(input+offset, FIELD_SIZE);
}


std::string Transcript::getHashData()
{
    std::string ret;

    for (int i = 0; i < r_size_; i++) {
        ret += CryptoPP::IntToString<CryptoPP::Integer>(r_[i].x);
        ret += CryptoPP::IntToString<CryptoPP::Integer>(r_[i].y);
    }
    ret += CryptoPP::IntToString<CryptoPP::Integer>(e_);
    ret += CryptoPP::IntToString<CryptoPP::Integer>(s_);

    return ret;
}