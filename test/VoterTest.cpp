#include "VoterTest.hpp"


void TestVoting()
{
    auto ecg = GenerateECGroup();

    auto gen = ecg.base;

    auto id_key = RandomInteger(2, ecg.order);
    auto id = ecg.curve.Multiply(id_key, gen);
    auto id_sum = ecg.curve.Multiply(27, gen);
    IDInfo id_info = {id, id_sum, id_key};

    std::vector<VoteTokenInfo> token_info;
    for (int i = 0; i < 20; i++) {
        auto token_key = RandomInteger(2, ecg.order);
        auto token = ecg.curve.Multiply(token_key, gen);
        auto token_sum = ecg.curve.Multiply(42+i, gen);
        token_info.push_back({token, token_sum, token_key});
    }

    Voter voter(ecg, gen, id_info, token_info);

    voter.castVote(8);
    Vote vote = voter.getVoteAndProofs();

    // vote.votes[2].x += 1;

    for (int i = 0; i < 20; i++) {
        ElGamalProtocol prot0(ecg, gen, id_info.id_sum, 0, token_info[i].token, vote.votes[i]);
        ElGamalProtocol prot1(ecg, gen, id_info.id_sum, 1, token_info[i].token, vote.votes[i]);
        std::vector<SigmaProtocol*> prots = {&prot0, &prot1};
        OrProtocol prot(prots);

        assert(prot.verifyNIZKP(vote.proofs[i]) == true);
    }
}