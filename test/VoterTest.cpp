#include "VoterTest.hpp"


void TestVoting()
{
    auto ecg = GenerateECGroup();

    auto gen = ecg.base;

    auto id_sum = ecg.curve.Multiply(27, gen);

    std::vector<CryptoPP::Integer> token_keys;
    std::vector<CryptoPP::ECPPoint> tokens;
    for (int i = 0; i < 20; i++) {
        token_keys.push_back(RandomInteger(2, ecg.order));
        tokens.push_back(ecg.curve.Multiply(token_keys[i], gen));
    }

    Voter voter(ecg, gen, id_sum, tokens);
    voter.setTokenKeys(token_keys);
    voter.castVote(8);
    auto votes = voter.getVoteAndProofs();

    // votes[8].vote.x += 1;

    for (int i = 0; i < 20; i++) {
        ElGamalProtocol prot0(ecg, gen, id_sum, 0, tokens[i], votes[i].vote);
        ElGamalProtocol prot1(ecg, gen, id_sum, 1, tokens[i], votes[i].vote);
        std::vector<SigmaProtocol*> prots = {&prot0, &prot1};
        OrProtocol prot(prots);

        assert(prot.verifyNIZKP(votes[i].proof) == true);
    }
}