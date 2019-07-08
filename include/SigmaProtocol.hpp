#ifndef SIGMA_HPP
#define SIGMA_HPP


#include "ECGroup.hpp"
#include <cryptlib.h>
#include <sha3.h>
#include <assert.h>


class SigmaProtocol {
public:
    virtual void generateCommitment() = 0;
    virtual void generateChallenge(CryptoPP::Integer* e = nullptr) = 0;
    virtual bool generateResponse() = 0;
    virtual bool verify() = 0;
    virtual bool generateSimulation() = 0;
    
    virtual CryptoPP::Integer challengeSize() = 0;
    virtual std::string getHashData() = 0;

    CryptoPP::Integer challenge() { return _e; }
    bool produceNIZKP();

protected:
    CryptoPP::Integer _e = 0;

private:
    CryptoPP::Integer genHashChallenge(const std::string& hash_data);
};


#endif