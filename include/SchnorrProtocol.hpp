#ifndef SCHNORR_PROTOCOL_HPP
#define SCHNORR_PROTOCOL_HPP


#include "SigmaProtocol.hpp"
#include <stdlib.h>
#include <time.h>
#include <math.h>


class SchnorrProtocol : public SigmaProtocol {
public:
    SchnorrProtocol(CryptoPP::ECP curve, 
                    CryptoPP::ECPPoint base,
                    CryptoPP::Integer order,  
                    CryptoPP::ECPPoint public_key, 
                    CryptoPP::Integer witness);
    SchnorrProtocol(CryptoPP::ECP curve, 
                    CryptoPP::ECPPoint base, 
                    CryptoPP::Integer order, 
                    CryptoPP::ECPPoint public_key);
    void generateCommitment() override;
    void generateChallenge(CryptoPP::Integer* e = nullptr) override;
    bool generateResponse() override;
    bool verify() override;
    bool generateSimulation() override;

    CryptoPP::Integer challengeSize() override { return _order; }

private:
    CryptoPP::ECP _curve;
    CryptoPP::ECPPoint _base;
    CryptoPP::Integer _order;
    CryptoPP::ECPPoint _pub_key;
    CryptoPP::Integer _w;
    CryptoPP::Integer _u;
    CryptoPP::ECPPoint _commitment;
    CryptoPP::Integer _s;
};


#endif