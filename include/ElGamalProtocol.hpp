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
                    int message);
    ~ElGamalProtocol() override {}
    
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
    const int m_;

    CryptoPP::ECPPoint gen2_;
    CryptoPP::ECPPoint pub_key1_;
    CryptoPP::ECPPoint pub_key2_;
    CryptoPP::Integer w_;

    CryptoPP::ECPPoint computeCommitment1();
    CryptoPP::ECPPoint computeCommitment2();
};


#endif