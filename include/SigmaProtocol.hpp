#ifndef SIGMA_HPP
#define SIGMA_HPP


#include "ECGroup.hpp"
#include <cryptlib.h>
#include <sha3.h>
#include <assert.h>


struct Transcript {
    std::vector<CryptoPP::ECPPoint> commitment;
    CryptoPP::Integer challenge;
    CryptoPP::Integer response;
};


class SigmaProtocol {
public:
    virtual void generateCommitment() = 0;
    virtual void generateChallenge(CryptoPP::Integer* e = nullptr) = 0;
    virtual void generateResponse() = 0;
    virtual bool verify() = 0;
    virtual void generateSimulation() = 0;
    
    virtual CryptoPP::Integer challengeSize() = 0;
    virtual std::string getHashData() = 0;

    virtual std::vector<CryptoPP::ECPPoint> commitment() = 0;
    virtual CryptoPP::Integer challenge() = 0;
    virtual CryptoPP::Integer response() = 0;

    Transcript getTranscript();
    Transcript generateNIZKP();
    virtual bool verifyTranscript(const Transcript& transcript) = 0;
    bool verifyNIZKP(const Transcript& nizkp);
};


CryptoPP::Integer GenHashChallenge(const std::string& hash_data,
                                   const CryptoPP::Integer& challenge_max);


#endif