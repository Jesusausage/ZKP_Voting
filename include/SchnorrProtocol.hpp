#ifndef SCHNORR_PROTOCOL_HPP
#define SCHNORR_PROTOCOL_HPP


#include "SigmaProtocol.hpp"
#include <stdlib.h>
#include <time.h>
#include <math.h>


class SchnorrProtocol : public SigmaProtocol {
public:
    SchnorrProtocol(const ECGroup& ecg,
                    const CryptoPP::ECPPoint& public_key, 
                    const CryptoPP::Integer& witness);
    SchnorrProtocol(const ECGroup& ecg,
                    const CryptoPP::ECPPoint& public_key);
    void generateCommitment() override;
    void generateChallenge(CryptoPP::Integer* e = nullptr) override;
    bool generateResponse() override;
    bool verify() override;
    bool generateSimulation() override;

    CryptoPP::Integer challengeSize() override { return *_order; }
    std::string getHashData() override;

    std::vector<CryptoPP::ECPPoint> commitment() override;
    CryptoPP::Integer response() override;

    bool verifyNIZKP(const NIZKP& nizkp) override;

private:
    const CryptoPP::ECP* _curve;
    const CryptoPP::ECPPoint* _base;
    const CryptoPP::Integer* _order;
    CryptoPP::ECPPoint _pub_key;
    CryptoPP::Integer _w;
    CryptoPP::Integer _u;
    CryptoPP::ECPPoint _commitment;
    CryptoPP::Integer _s;
};


#endif