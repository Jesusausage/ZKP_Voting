#include "VoterTest.hpp"


void TestVoting()
{
    auto ecg = GenerateECGroup();

    auto gen = ecg.base;

    auto id_sum = ecg.curve.Multiply(27, gen);

    std::vector<CryptoPP::Integer> token_keys;
    std::vector<CryptoPP::ECPPoint> tokens;
    for (int i = 0; i < 10; i++) {
        token_keys.push_back(RandomInteger(2, ecg.order));
        tokens.push_back(ecg.curve.Multiply(token_keys[i], gen));
    }

    Voter voter(ecg, gen, id_sum, tokens);
    voter.setTokenKeys(token_keys);
    voter.castVote(8);
    Vote votes = voter.getVoteAndProofs();
     
    // votes.values[8].x += 1;

    for (int i = 0; i < 10; i++) {
        ElGamalProtocol prot0(ecg, gen, 0);
        ElGamalProtocol prot1(ecg, gen, 1);
        prot0.setParams(id_sum, tokens[i], votes.values[i]);
        prot1.setParams(id_sum, tokens[i], votes.values[i]);

        OrProtocol prot({&prot0, &prot1});
        assert(prot.verifyNIZKP(votes.proofs[i]) == true);
    }
}