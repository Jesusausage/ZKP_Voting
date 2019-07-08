#ifndef SIGMA_HPP
#define SIGMA_HPP


#include "ECGroup.hpp"


class SigmaProtocol {
public:
    virtual void generateCommitment() = 0;
    virtual void generateChallenge(CryptoPP::Integer* e = nullptr) = 0;
    virtual bool generateResponse() = 0;
    virtual bool verify() = 0;
    virtual bool generateSimulation() = 0;
    
    virtual CryptoPP::Integer challengeSize() = 0;

    CryptoPP::Integer challenge() { return _e; }

protected:
    CryptoPP::Integer _e = 0;
};


#endif