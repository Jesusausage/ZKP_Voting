#ifndef VERIFIER_HPP
#define VERIFIER_HPP


#include "OrProtocol.hpp"
#include "ElGamalProtocol.hpp"
#include "Voter.hpp"


class Verifier {
public:
    Verifier(const ECGroup& ecg,
          const CryptoPP::ECPPoint& generator,
          const CryptoPP::ECPPoint& id_sum);

    void setVoterTokens(const std::vector<CryptoPP::ECPPoint>& tokens);
    bool verifyProofs(const std::vector<Vote>& votes);

private:
    const ECGroup* _ecg;
    const CryptoPP::ECPPoint _gen;
    const CryptoPP::ECPPoint _id_sum;

    std::vector<CryptoPP::ECPPoint> _tokens;
};


#endif