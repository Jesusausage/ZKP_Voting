#ifndef OR_PROTOCOL_HPP
#define OR_PROTOCOL_HPP


#include "SigmaProtocol.hpp"
#include <stdlib.h>
#include <time.h>
#include <math.h>


void f();


struct OrNIZKP {
    std::vector<Transcript> transcripts;
    CryptoPP::Integer e;
};


class OrProtocol {
public:
    OrProtocol(std::vector<SigmaProtocol*> sigma_protocols, int i_known);
    void generateCommitment();
    void generateChallenge(CryptoPP::Integer e);
    void generateResponse();
    bool verify();
    //void generateSimulation();

    CryptoPP::Integer challengeSize();
    std::string getHashData();

    void generateNIZKP();
    OrNIZKP getNIZKP();
    bool verifyNIZKP(const OrNIZKP& or_nizkp);

private:
    std::vector<SigmaProtocol*> _sigma_prots;
    int _i_known;
    int _num_prots;
    CryptoPP::Integer _total_e = 0;
    CryptoPP::Integer _e = 0;
    std::vector<Transcript> _transcripts;

    CryptoPP::Integer _genHashChallenge(const std::string& hash_data);
};


#endif