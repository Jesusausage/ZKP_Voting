#include "VerifierTest.hpp"


void TestVerification()
{
    auto ecg = GenerateECGroup();
    auto gen = ecg.base;

    auto id_key = RandomInteger(2, ecg.order);
    auto id = ecg.curve.Multiply(id_key, gen);
    auto id_sum = id;

    std::vector<CryptoPP::Integer> token_keys;
    std::vector<CryptoPP::ECPPoint> tokens;
    for (int i = 0; i < 10; i++) {
        token_keys.push_back(RandomInteger(2, ecg.order));
        tokens.push_back(ecg.curve.Multiply(token_keys[i], gen));
    }
    std::vector<CryptoPP::ECPPoint> token_sums = tokens;

    Voter voter(ecg, gen, id_sum, tokens);
    voter.setTokenKeys(token_keys);
    voter.castVote(8);
    Vote votes = voter.getVoteAndProofs();    
    // votes.values[8].x += 1;

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


void TestVoteDecryption()
{
    auto ecg = GenerateECGroup();
    auto gen = ecg.base;

    const int no_voters = 10;

    CryptoPP::Integer id_key[no_voters];
    CryptoPP::ECPPoint id[no_voters];
    CryptoPP::ECPPoint id_sum;
    for (int i = 0; i < no_voters; i++) {
        id_key[i] = RandomInteger(2, ecg.order);
        id[i] = ecg.curve.Multiply(id_key[i], gen);
        id_sum = ecg.curve.Add(id[i], id_sum);
    }

    std::vector<std::vector<CryptoPP::Integer>> token_keys;
    std::vector<std::vector<CryptoPP::ECPPoint>> tokens;
    std::vector<CryptoPP::ECPPoint> token_sums;
    token_keys.resize(no_voters);
    tokens.resize(no_voters);
    token_sums.resize(6);
    for (int i = 0; i < no_voters; i++) {
        token_keys[i].resize(6);
        tokens[i].resize(6);
        for (int j = 0; j < 6; j++) {
            token_keys[i][j] = RandomInteger(2, ecg.order);
            tokens[i][j] = ecg.curve.Multiply(token_keys[i][j], gen);
            token_sums[j] = ecg.curve.Add(tokens[i][j], token_sums[j]);
        }
    } 

    Vote votes[no_voters];
    Key keys[no_voters];

    for (int i = 0; i < no_voters; i++) {
        Voter voter(ecg, gen, id_sum, tokens[i]);
        voter.setTokenKeys(token_keys[i]);
        voter.castVote(2 + i%2);
        votes[i] = voter.getVoteAndProofs();
        
        KeyGen key_gen(ecg, gen, token_sums, id[i]);
        key_gen.setIDKey(id_key[i]);
        keys[i] = key_gen.getKeysAndProofs();
    }

    CryptoPP::ECPPoint vote_t[6];
    CryptoPP::ECPPoint key_t[6];
    for (int j = 0; j < 6; j++) {
        for (int i = 0; i < no_voters; i++) {
            vote_t[j] = ecg.curve.Add(vote_t[j], votes[i].values[j]);
            key_t[j] = ecg.curve.Add(key_t[j], keys[i].values[j]);
        }
    }

    int results[6];
    for (int j = 0; j < 6; j++) {
        auto gm = ecg.curve.Add(vote_t[j], ecg.curve.Inverse(key_t[j]));

        int i = 0;
        while (!(gm == ecg.curve.Multiply(i, gen)))
            i++;
        results[j] = i;
    }

    assert(results[0] == 0);
    assert(results[1] == 0);
    assert(results[2] == 5);
    assert(results[3] == 5);
    assert(results[4] == 0);
    assert(results[5] == 0);
}


void TestVoteCompression()
{
    auto ecg = GenerateECGroup();
    auto gen = ecg.base;

    auto id_key = RandomInteger(2, ecg.order);
    auto id = ecg.curve.Multiply(id_key, gen);
    auto id_sum = id;

    std::vector<CryptoPP::Integer> token_keys;
    std::vector<CryptoPP::ECPPoint> tokens;
    for (int i = 0; i < 10; i++) {
        token_keys.push_back(RandomInteger(2, ecg.order));
        tokens.push_back(ecg.curve.Multiply(token_keys[i], gen));
    }
    std::vector<CryptoPP::ECPPoint> token_sums = tokens;

    Voter voter(ecg, gen, id_sum, tokens);
    voter.setTokenKeys(token_keys);
    voter.castVote(8);
    CompressedVote c_vote = voter.getCompressedVote();
    // c_vote.values[0].x += 1;

    KeyGen key_gen(ecg, gen, token_sums, id);
    key_gen.setIDKey(id_key);
    CompressedKey c_key = key_gen.getCompressedKey();
    // c_key.values[0].x += 1;

    Verifier verifier(ecg, gen, id_sum, token_sums);
    verifier.setVoterTokens(tokens);
    Vote vote = verifier.decompressVote(c_vote);
    assert(verifier.verifyVoteProofs(vote) == true);
    verifier.setID(id);
    Key key = verifier.decompressKey(c_key);
    assert(verifier.verifyKeyProofs(key) == true);
}