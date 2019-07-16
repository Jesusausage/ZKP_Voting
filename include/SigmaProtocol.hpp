#ifndef SIGMA_HPP
#define SIGMA_HPP


#include "ECGroup.hpp"
#include <cryptopp/cryptlib.h>
#include <cryptopp/sha3.h>
#include <assert.h>


struct Transcript {
    std::vector<CryptoPP::ECPPoint> commitment;
    CryptoPP::Integer challenge;
    CryptoPP::Integer response;
};


struct CompressedTranscript {
    std::vector<CompressedPoint> commitment;
    CryptoPP::Integer challenge;
    CryptoPP::Integer response;
};


class SigmaProtocol {
public:
    SigmaProtocol(const ECGroup& ecg, const CryptoPP::ECPPoint& generator);
    virtual ~SigmaProtocol() {}

    virtual void generateCommitment() = 0;
    void generateChallenge(CryptoPP::Integer* e = nullptr);
    virtual void generateResponse() = 0;
    virtual bool verify() = 0;
    virtual void generateSimulation() = 0;
    
    CryptoPP::Integer challengeSize();
    virtual std::string getHashData() = 0;

    std::vector<CryptoPP::ECPPoint> commitment();
    CryptoPP::Integer challenge();
    CryptoPP::Integer response();

    Transcript getTranscript();
    void setTranscript(const Transcript& transcript);
    
    Transcript generateNIZKP();
    bool verifyNIZKP(const Transcript& nizkp);

protected:    
    const CryptoPP::ECP* _curve;
    const CryptoPP::Integer* _order;
    const CryptoPP::ECPPoint* _gen;

    CryptoPP::Integer _u;
    std::vector<CryptoPP::ECPPoint> _commitment;
    CryptoPP::Integer _e = 0;
    CryptoPP::Integer _s;
};


CryptoPP::Integer GenHashChallenge(const std::string& hash_data,
                                   const CryptoPP::Integer& challenge_max);

CompressedTranscript CompressTranscript(const Transcript& transcript);

Transcript DecompressTranscript(const CompressedTranscript& compressed_transcript,
                                const CryptoPP::ECP& curve);


#endif