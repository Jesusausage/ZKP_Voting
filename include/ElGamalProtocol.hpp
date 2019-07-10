#ifndef ELGAMAL_PROTOCOL_HPP
#define ELGAMAL_PROTOCOL_HPP


#include "SigmaProtocol.hpp"
#include <stdlib.h>
#include <time.h>
#include <math.h>


class ElGamalProtocol : public SigmaProtocol {
public:
    ElGamalProtocol(const ECGroup& ecg,
                    const CryptoPP::ECPPoint& generator1, 
                    const CryptoPP::ECPPoint& generator2, 
                    const CryptoPP::ECPPoint& public_key1, 
                    const CryptoPP::ECPPoint& public_key2, 
                    const CryptoPP::Integer& witness);
    ElGamalProtocol(const ECGroup& ecg,
                    const CryptoPP::ECPPoint& generator1, 
                    const CryptoPP::ECPPoint& generator2, 
                    const CryptoPP::ECPPoint& public_key1, 
                    const CryptoPP::ECPPoint& public_key2);
    void generateCommitment() override;
    void generateChallenge(CryptoPP::Integer* e = nullptr) override;
    void generateResponse() override;
    bool verify() override;
    void generateSimulation() override;

    CryptoPP::Integer challengeSize() override { return *_order; }
    std::string getHashData() override;
    
    std::vector<CryptoPP::ECPPoint> commitment() override;
    CryptoPP::Integer challenge() override;
    CryptoPP::Integer response() override;    
    
    bool verifyTranscript(const Transcript& transcript) override;

private:
    const CryptoPP::ECP* _curve;
    const CryptoPP::Integer* _order;
    const CryptoPP::ECPPoint _gen1;
    const CryptoPP::ECPPoint _gen2;
    CryptoPP::ECPPoint _pub_key1;
    CryptoPP::ECPPoint _pub_key2;
    CryptoPP::Integer _w;    
    CryptoPP::Integer _u;
    CryptoPP::ECPPoint _commitment1;
    CryptoPP::ECPPoint _commitment2;
    CryptoPP::Integer _s;
};


#endif