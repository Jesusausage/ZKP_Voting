#ifndef OR_PROTOCOL_HPP
#define OR_PROTOCOL_HPP


#include "SigmaProtocol.hpp"
#include <stdlib.h>
#include <time.h>
#include <math.h>


struct OrNIZKP {
    std::vector<Transcript> transcripts;
    CryptoPP::Integer e;
};


struct CompressedOrNIZKP {
    std::vector<CompressedTranscript> transcripts;
    CryptoPP::Integer e;
};


class OrProtocol {
public:
    OrProtocol(std::vector<SigmaProtocol*> sigma_protocols, int i_known = 0);
    void setKnown(int i_known);
    void generateCommitment();
    void generateChallenge(CryptoPP::Integer e);
    void generateResponse();
    bool verify();
    //void generateSimulation();

    CryptoPP::Integer challengeSize();
    std::string getHashData();

    OrNIZKP generateNIZKP();
    bool verifyNIZKP(const OrNIZKP& or_nizkp);

private:
    std::vector<SigmaProtocol*> _sigma_prots;
    int _i_known;
    int _num_prots;
    CryptoPP::Integer _total_e = 0;
    CryptoPP::Integer _e = 0;
};


#endif