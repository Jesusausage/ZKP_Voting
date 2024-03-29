#ifndef TRANSCRIPT_HPP
#define TRANSCRIPT_HPP


#include "ECGroup.hpp"
#include <cryptopp/ecp.h>
#include <iostream>


class Transcript {
public:
    Transcript(CryptoPP::ECPPoint* commitment,
               int commitment_size,
               const CryptoPP::Integer& challenge,
               const CryptoPP::Integer& response);
    Transcript(const Transcript& transcript);
    Transcript(Transcript&& transcript);
    Transcript();
    ~Transcript();

    Transcript& operator=(Transcript transcript);
    friend void swap(Transcript& a, Transcript& b);

    void setCommitment(CryptoPP::ECPPoint* commitment, 
                       int commitment_size);
    inline void setChallenge(const CryptoPP::Integer& challenge) 
        { e_ = challenge; }
    inline void setResponse(const CryptoPP::Integer& response) 
        { s_ = response; }

    inline const CryptoPP::ECPPoint& commitment(int i) const { return r_[i]; }
    inline const int& commitmentSize() const { return r_size_; }
    inline const CryptoPP::Integer& challenge() const { return e_; }
    inline const CryptoPP::Integer& response() const { return s_; }

    void serialise(CryptoPP::byte* output, 
                   int* commitment_size = nullptr);
    Transcript(CryptoPP::byte* input, 
               int commitment_size,
               const CryptoPP::ECP& ec);

    std::string getHashData();

private:
    CryptoPP::ECPPoint* r_ = nullptr;
    int r_size_ = 0;
    CryptoPP::Integer e_ = 0;
    CryptoPP::Integer s_ = 0;
};


#endif