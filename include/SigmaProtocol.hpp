#ifndef SIGMA_HPP
#define SIGMA_HPP


#include "Group.hpp"


class SigmaProtocol {
public:
    SigmaProtocol(Group<ModularInt> group) : G(group)
    {}
    virtual void generateCommitment() = 0;
    virtual void generateChallenge(int e = 0) = 0;
    virtual bool generateResponse() = 0;
    virtual bool verify() = 0;
    virtual bool generateSimulation() = 0;

protected:
    Group<ModularInt> G;
};


#endif