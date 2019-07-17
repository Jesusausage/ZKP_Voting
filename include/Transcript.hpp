#ifndef TRANSCRIPT_HPP
#define TRANSCRIPT_HPP


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

    inline CryptoPP::ECPPoint* commitment() const { return r_; }
    inline int commitmentSize() const { return r_size_; }
    inline CryptoPP::Integer challenge() const { return e_; }
    inline CryptoPP::Integer response() const { return s_; }

private:
    CryptoPP::ECPPoint* r_ = nullptr;
    int r_size_ = 0;
    CryptoPP::Integer e_ = 0;
    CryptoPP::Integer s_ = 0;
};


#endif