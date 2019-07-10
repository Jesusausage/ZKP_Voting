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
                    const CryptoPP::Integer& witness);
    SchnorrProtocol(const ECGroup& ecg,
                    const CryptoPP::ECPPoint& generator, 
                    const CryptoPP::ECPPoint& public_key);
    void generateCommitment() override;
    void generateChallenge(CryptoPP::Integer* e = nullptr) override;
    void generateResponse() override;
    bool verify() override;
    void generateSimulation() override;

    CryptoPP::Integer challengeSize() override;
    std::string getHashData() override;
    
    std::vector<CryptoPP::ECPPoint> commitment() override;
    CryptoPP::Integer challenge() override;
    CryptoPP::Integer response() override;    
    
    bool verifyTranscript(const Transcript& transcript) override;

private:
    const CryptoPP::ECP* _curve;
    const CryptoPP::Integer* _order;
    const CryptoPP::ECPPoint _gen;

    CryptoPP::ECPPoint _pub_key;
    CryptoPP::Integer _w;

    CryptoPP::Integer _u;
    CryptoPP::ECPPoint _commitment;
    CryptoPP::Integer _e = 0;
    CryptoPP::Integer _s;
};


#endif