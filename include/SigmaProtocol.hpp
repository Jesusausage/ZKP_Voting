#ifndef SIGMA_HPP
#define SIGMA_HPP


#include "Group.hpp"


class SigmaProtocol {
public:
    virtual void generateCommitment() = 0;
    virtual void generateChallenge(int* e = nullptr) = 0;
    virtual bool generateResponse() = 0;
    virtual bool verify() = 0;
    virtual bool generateSimulation() = 0;
    
    virtual int challengeSize() = 0;

    int challenge() { return _e; }

protected:
    int _e = 0;
};


#endif