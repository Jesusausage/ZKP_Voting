#include "Verifier.hpp"


Verifier::Verifier(const ECGroup& ecg,
                   const CryptoPP::ECPPoint& generator,
                   const CryptoPP::ECPPoint& id_sum)
                   :
                   _ecg(&ecg),
                   _gen(generator),
                   _id_sum(id_sum)
{
    _prots[0] = new ElGamalProtocol(*_ecg, _gen, _id_sum, 0);
    _prots[1] = new ElGamalProtocol(*_ecg, _gen, _id_sum, 1);
    _or_prot = new OrProtocol({_prots[0], _prots[1]});
}


Verifier::~Verifier()
{
    delete _prots[0];
    delete _prots[1];
    delete _or_prot;
}


void Verifier::setVoterTokens(const std::vector<CryptoPP::ECPPoint>& tokens)
{
    _tokens = tokens;
}


bool Verifier::verifyProofs(const std::vector<Vote>& votes)
{
    int num_options = votes.size();

    for (int i = 0; i < num_options; i++) {
        _prots[0]->setKeys(_tokens[i], votes[i].vote);
        _prots[1]->setKeys(_tokens[i], votes[i].vote);

        if (_or_prot->verifyNIZKP(votes[i].proof) == false)
            return false;
    }

    return true;
}