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
    Vote vote = voter.getVoteAndProofs();

    CryptoPP::byte* o;
    int n;
    vote.serialise(o, n);
    Vote ote(o, n, ecg.curve);
    delete [] o;

    // for (int i = 0; i < 10; i++) {
    //     std::cout << i << std::endl;
    //     assert(vote.value(i).x == ote.value(i).x);
    //     assert(vote.value(i).y == ote.value(i).y);

    //     auto a = vote.proof(i);
    //     auto b = ote.proof(i);
    //     std::cout << a.transcript(0).challenge().MinEncodedSize() << std::endl;
    //     std::cout << a.transcript(1).challenge().MinEncodedSize() << std::endl;
    //     assert(a.e() == b.e());
    //     assert(a.transcript(0).challenge() == b.transcript(0).challenge());
    //     assert(a.transcript(0).response() == b.transcript(0).response());
    //     assert(a.transcript(1).challenge() == b.transcript(1).challenge());
    //     assert(a.transcript(1).response() == b.transcript(1).response());
    // }

    for (int i = 0; i < 10; i++) {
        ElGamalProtocol prot0(ecg, gen, 0);
        ElGamalProtocol prot1(ecg, gen, 1);
        prot0.setParams(id_sum, tokens[i], ote.value(i));
        prot1.setParams(id_sum, tokens[i], ote.value(i));

        OrProtocol prot({&prot0, &prot1});
        assert(prot.verifyNIZKP(ote.proof(i)) == true);
    }
}