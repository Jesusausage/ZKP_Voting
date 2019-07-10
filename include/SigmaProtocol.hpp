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

    CryptoPP::Integer challenge() { return _e; }

    virtual void generateNIZKP() = 0;
    Transcript getNIZKP() { return _nizkp; }
    virtual bool verifyNIZKP(const Transcript& nizkp) = 0;

    virtual Transcript getTranscript() = 0;
    virtual bool verifyTranscript(const Transcript& transcript) = 0;

protected:
    CryptoPP::Integer _e = 0;
    Transcript _nizkp;
    Transcript _transcript;
};


CryptoPP::Integer GenHashChallenge(const std::string& hash_data,
                                   const CryptoPP::Integer& challenge_max);


#endif