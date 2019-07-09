#ifndef SIGMA_HPP
#define SIGMA_HPP


#include "ECGroup.hpp"
#include <cryptlib.h>
#include <sha3.h>
#include <assert.h>
#include <vector>


struct NIZKP {
    std::vector<CryptoPP::ECPPoint> commitment;
    CryptoPP::Integer challenge;
    CryptoPP::Integer response;
};


class SigmaProtocol {
public:
    virtual void generateCommitment() = 0;
    virtual void generateChallenge(CryptoPP::Integer* e = nullptr) = 0;
    virtual bool generateResponse() = 0;
    virtual bool verify() = 0;
    virtual bool generateSimulation() = 0;
    
    virtual CryptoPP::Integer challengeSize() = 0;
    virtual std::string getHashData() = 0;

    virtual std::vector<CryptoPP::ECPPoint> commitment() = 0; //f
    CryptoPP::Integer challenge() { return _e; }
    virtual CryptoPP::Integer response() = 0; //f

    NIZKP produceNIZKP();
    virtual bool verifyNIZKP(const NIZKP& nizkp); //f

protected:
    CryptoPP::Integer _e = 0;

    CryptoPP::Integer genHashChallenge(const std::string& hash_data);
};


#endif