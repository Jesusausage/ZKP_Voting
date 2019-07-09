#ifndef OR_PROTOCOL_HPP
#define OR_PROTOCOL_HPP


#include "SigmaProtocol.hpp"
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <vector>


class OrProtocol {
public:
    OrProtocol(std::vector<SigmaProtocol*> sigma_protocols, int i_known);
    void generateCommitment();
    void generateChallenge(CryptoPP::Integer e);
    void generateResponse();
    bool verify();
    //void generateSimulation();

    std::string getHashData();

    void generateNIZKP();



private:
    std::vector<SigmaProtocol*> _sigma_prots;
    int _i_known;
    int _num_prots;
    CryptoPP::Integer _total_e = 0;
    CryptoPP::Integer _e = 0;
};


#endif