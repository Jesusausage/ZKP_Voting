#ifndef SCHNORR_PROTOCOL_HPP
#define SCHNORR_PROTOCOL_HPP


#include "SigmaProtocol.hpp"
#include <stdlib.h>
#include <time.h>
#include <math.h>


class SchnorrProtocol : public SigmaProtocol {
public:
    SchnorrProtocol(Group<ModularInt> group, 
                    ModularInt generator, 
                    ModularInt public_key, 
                    int witness);
    SchnorrProtocol(Group<ModularInt> group, 
                    ModularInt generator, 
                    ModularInt public_key);
    void generateCommitment() override;
    void generateChallenge(int e = 0) override;
    bool generateResponse() override;
    bool verify() override;
    bool generateSimulation() override;

private:
    ModularInt _g;
    ModularInt _pub_key;
    int _w;
    int _u;
    ModularInt _commitment;
    int _e = 0;
    int _s;
};


#endif