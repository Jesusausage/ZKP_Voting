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
        { _e = challenge; }
    inline void setResponse(const CryptoPP::Integer& response) 
        { _s = response; }

    inline CryptoPP::ECPPoint* commitment() const { return _r; }
    inline int commitmentSize() const { return _r_size; }
    inline CryptoPP::Integer challenge() const { return _e; }
    inline CryptoPP::Integer response() const { return _s; }

private:
    CryptoPP::ECPPoint* _r = nullptr;
    int _r_size = 0;
    CryptoPP::Integer _e = 0;
    CryptoPP::Integer _s = 0;
};


#endif