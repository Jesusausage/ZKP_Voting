#include "Verifier.hpp"


Verifier::Verifier(const ECGroup& ecg,
                   const CryptoPP::ECPPoint& generator,
                   const CryptoPP::ECPPoint& id_sum)
                   :
                   _ecg(&ecg),
                   _gen(generator),
                   _id_sum(id_sum)
{}


void Verifier::setVoterTokens(const std::vector<CryptoPP::ECPPoint>& tokens)
{
    _tokens = tokens;
}


bool Verifier::verifyProofs(const std::vector<Vote>& votes)
{
    int num_options = votes.size();
    for (int i = 0; i < num_options; i++) {
        ElGamalProtocol prot0(*_ecg, _gen, _id_sum, 0, _tokens[i], votes[i].vote);
        ElGamalProtocol prot1(*_ecg, _gen, _id_sum, 1, _tokens[i], votes[i].vote);
        std::vector<SigmaProtocol*> prots = {&prot0, &prot1};
        OrProtocol prot(prots);

        if (prot.verifyNIZKP(votes[i].proof) == false)
            return false;
    }

    return true;
}