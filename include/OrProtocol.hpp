#ifndef OR_PROTOCOL_HPP
#define OR_PROTOCOL_HPP


#include "SigmaProtocol.hpp"
#include "OrTranscript.hpp"
#include <stdlib.h>
#include <time.h>
#include <math.h>


class OrProtocol {
public:
    OrProtocol(SigmaProtocol** sigma_protocols, int num_prots, int i_known = 0);
    ~OrProtocol();
    void setKnown(int i_known);

    void generateCommitment();
    void generateChallenge(const CryptoPP::Integer& e);
    void generateResponse();
    bool verify();

    std::string getHashData();

    OrTranscript generateNIZKP();
    bool verifyNIZKP(const OrTranscript& or_nizkp);

private:
    SigmaProtocol** sigma_prots_;
    int num_prots_;
    int i_known_;

    CryptoPP::Integer e_ = 0;
    CryptoPP::Integer total_e_ = 0;
    CryptoPP::Integer e_size_ = 0;
};


#endif