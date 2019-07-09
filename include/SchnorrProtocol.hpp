#ifndef SCHNORR_PROTOCOL_HPP
#define SCHNORR_PROTOCOL_HPP


#include "SigmaProtocol.hpp"
#include <stdlib.h>
#include <time.h>
#include <math.h>


struct SchnorrNIZKP {
    CryptoPP::ECPPoint commitment;
    CryptoPP::Integer challenge;
    CryptoPP::Integer response;
};


class SchnorrProtocol : public SigmaProtocol {
public:
    SchnorrProtocol(const ECGroup& ecg,
                    const CryptoPP::ECPPoint& public_key, 
                    const CryptoPP::Integer& witness);
    SchnorrProtocol(const ECGroup& ecg,
                    const CryptoPP::ECPPoint& public_key);
    void generateCommitment() override;
    void generateChallenge(CryptoPP::Integer* e = nullptr) override;
    void generateResponse() override;
    bool verify() override;
    void generateSimulation() override;

    CryptoPP::Integer challengeSize() override { return *_order; }
    std::string getHashData() override;
    
    CryptoPP::ECPPoint commitment() { return _commitment; }
    CryptoPP::Integer response() { return _s; }

    void generateNIZKP() override;
    SchnorrNIZKP getNIZKP() { return _nizkp; }
    bool verifyNIZKP(const SchnorrNIZKP& nizkp);

private:
    const CryptoPP::ECP* _curve;
    const CryptoPP::ECPPoint* _base;
    const CryptoPP::Integer* _order;
    CryptoPP::ECPPoint _pub_key;
    CryptoPP::Integer _w;    
    CryptoPP::Integer _u;
    CryptoPP::ECPPoint _commitment;
    CryptoPP::Integer _s;
    SchnorrNIZKP _nizkp;
};


#endif