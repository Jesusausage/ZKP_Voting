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
    Transcript();
    ~Transcript();

    void setCommitment(CryptoPP::ECPPoint* commitment, 
                       int commitment_size);
    void setChallenge(const CryptoPP::Integer& challenge);
    void setResponse(const CryptoPP::Integer& response);

    CryptoPP::ECPPoint* commitment() const;
    int commitmentSize() const;
    CryptoPP::Integer challenge() const;
    CryptoPP::Integer response() const;

private:
    CryptoPP::ECPPoint* _r = nullptr;
    int _r_size = 0;
    CryptoPP::Integer _e = 0;
    CryptoPP::Integer _s = 0;
};


#endif