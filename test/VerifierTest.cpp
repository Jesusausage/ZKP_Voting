#include "VerifierTest.hpp"


void TestVerification()
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

    auto id_key = RandomInteger(2, ecg.order);
    auto id = ecg.curve.Multiply(id_key, gen);
    std::vector<CryptoPP::ECPPoint> token_sums;
    for (int i = 0; i < 10; i++) {
        auto x = RandomInteger(2, ecg.order);
        token_sums.push_back(ecg.curve.Multiply(x, gen));
    }
    KeyGen key_gen(ecg, gen, token_sums, id);
    key_gen.setIDKey(id_key);
    Key keys = key_gen.getKeysAndProofs();
    // keys.values[8].x += 1;

    Verifier verifier(ecg, gen, id_sum, token_sums);
    verifier.setVoterTokens(tokens);
    assert(verifier.verifyVoteProofs(votes) == true);
    verifier.setID(id);
    assert(verifier.verifyKeyProofs(keys) == true);
}