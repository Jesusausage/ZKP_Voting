#ifndef OR_PROTOCOL_HPP
#define OR_PROTOCOL_HPP


#include "SigmaProtocol.hpp"
#include <stdlib.h>
#include <time.h>
#include <math.h>


class OrProtocol : public SigmaProtocol {
public:
    OrProtocol(SigmaProtocol* sigma0, SigmaProtocol* sigma1, bool know0);
    void generateCommitment() override;
    void generateChallenge(CryptoPP::Integer* e = nullptr) override;
    bool generateResponse() override;
    bool verify() override;
    bool generateSimulation() override;

    CryptoPP::Integer challengeSize() override { return _e_size; }
    std::string getHashData() override;

    std::vector<CryptoPP::ECPPoint> commitment() override;
    CryptoPP::Integer response() override;

    bool verifyNIZKP(const NIZKP& nizkp) override;

private:
    SigmaProtocol* _sigma0;
    SigmaProtocol* _sigma1;
    bool _know0;
    CryptoPP::Integer _e_size = 0;
};


#endif