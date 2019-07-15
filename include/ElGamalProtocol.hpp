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
                    int message);
    ElGamalProtocol(const ECGroup& ecg,
                    const CryptoPP::ECPPoint& generator1,
                    int message);
    ~ElGamalProtocol() override {}
    void setKeys(const CryptoPP::ECPPoint& public_key1,
                 const CryptoPP::ECPPoint& public_key2,
                 const CryptoPP::Integer& witness = 0);
    void setParams(const CryptoPP::ECPPoint& generator2,
                   const CryptoPP::ECPPoint& public_key1,
                   const CryptoPP::ECPPoint& public_key2,
                   const CryptoPP::Integer& witness = 0);
    void generateCommitment() override;
    void generateResponse() override;
    bool verify() override;
    void generateSimulation() override;

    std::string getHashData() override; 

private:
    const int _m;

    CryptoPP::ECPPoint _gen2;
    CryptoPP::ECPPoint _pub_key1;
    CryptoPP::ECPPoint _pub_key2;
    CryptoPP::Integer _w;

    CryptoPP::ECPPoint computeCommitment1();
    CryptoPP::ECPPoint computeCommitment2();
};


#endif