#ifndef SCHNORR_PROTOCOL_HPP
#define SCHNORR_PROTOCOL_HPP


#include "SigmaProtocol.hpp"
#include <stdlib.h>
#include <time.h>
#include <math.h>


class SchnorrProtocol : public SigmaProtocol {
public:
    SchnorrProtocol(const ECGroup& ecg,
                    const CryptoPP::ECPPoint& generator, 
                    const CryptoPP::ECPPoint& public_key, 
                    const CryptoPP::Integer& witness = 0);
    ~SchnorrProtocol() override {}
    
    void generateCommitment() override;
    void generateResponse() override;
    bool verify() override;
    void generateSimulation() override;

    std::string getHashData() override;

private:
    CryptoPP::ECPPoint _pub_key;
    CryptoPP::Integer _w;
};


#endif