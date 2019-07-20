#ifndef OR_PROTOCOL_HPP
#define OR_PROTOCOL_HPP


#include "SigmaProtocol.hpp"
#include "OrTranscript.hpp"
#include <stdlib.h>
#include <time.h>
#include <math.h>


class OrProtocol {
public:
    OrProtocol(std::vector<SigmaProtocol*> sigma_protocols, int i_known = 0);
    void setKnown(int i_known);
    void generateCommitment();
    void generateChallenge(const CryptoPP::Integer& e);
    void generateResponse();
    bool verify();

    std::string getHashData();

    OrTranscript generateNIZKP();
    bool verifyNIZKP(const OrTranscript& or_nizkp);

private:
    std::vector<SigmaProtocol*> sigma_prots_;
    int i_known_;
    int num_prots_;

    CryptoPP::Integer e_ = 0;
    CryptoPP::Integer total_e_ = 0;
    CryptoPP::Integer e_size_ = 0;
};


#endif