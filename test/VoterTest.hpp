#ifndef VOTER_TEST_HPP
#define VOTER_TEST_HPP


#include "Voter.hpp"
#include <assert.h>
#include <iostream>


void TestVoting()
{
    auto ecg = GenerateECGroup();

    auto gen = ecg.base;

    auto id_key = RandomInteger(2, ecg.order);
    auto id = ecg.curve.Multiply(id_key, gen);
    auto id_sum = ecg.curve.Multiply(27, gen);
    IDInfo id_info = {id, id_sum, id_key};

    std::vector<VoteTokenInfo> token_info;
    for (int i = 0; i < 4; i++) {
        auto token_key = RandomInteger(2, ecg.order);
        auto token = ecg.curve.Multiply(token_key, gen);
        auto token_sum = ecg.curve.Multiply(42+i, gen);
        token_info.push_back({token, token_sum, token_key});
    }

    Voter voter(ecg, gen, id_info, token_info);

    voter.castVote(0);
    Vote vote = voter.getVoteAndProofs();
}


#endif
