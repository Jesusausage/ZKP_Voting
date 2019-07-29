#include "VerifierTest.hpp"


void TestVerification()
{
    auto ecg = GenerateECGroup();
    auto gen = GenerateECBase();

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
    Vote vote = voter.getVoteAndProofs();

    KeyGen key_gen(ecg, gen, token_sums, id);
    key_gen.setIDKey(id_key);
    Key key = key_gen.getKeysAndProofs();

    Verifier verifier(ecg, gen, id_sum, token_sums);
    verifier.setTokens(tokens);
    assert(verifier.verifyVote(vote) == true);
    verifier.setID(id);
    assert(verifier.verifyKey(key) == true);
}


void TestVoteDecryption()
{
    auto ecg = GenerateECGroup();
    auto gen = GenerateECBase();

    const int no_voters = 4;

    CryptoPP::Integer id_key[no_voters];
    CryptoPP::ECPPoint id[no_voters];
    CryptoPP::ECPPoint id_sum;
    for (int i = 0; i < no_voters; i++) {
        id_key[i] = RandomInteger(2, ecg.order);
        id[i] = ecg.curve.Multiply(id_key[i], gen);
        id_sum = ecg.curve.Add(id[i], id_sum);
    }

    CryptoPP::Integer token_keys[no_voters][6];
    CryptoPP::ECPPoint tokens[no_voters][6];
    CryptoPP::ECPPoint token_sums[6];
    for (int i = 0; i < no_voters; i++) {
        for (int j = 0; j < 6; j++) {
            token_keys[i][j] = RandomInteger(2, ecg.order);
            tokens[i][j] = ecg.curve.Multiply(token_keys[i][j], gen);
            token_sums[j] = ecg.curve.Add(tokens[i][j], token_sums[j]);
        }
    } 

    Vote vote[no_voters];
    Key keys[no_voters];

    for (int i = 0; i < no_voters; i++) {
        Voter voter(ecg, gen, id_sum, tokens[i], 6);
        voter.setTokenKeys(token_keys[i]);
        voter.castVote(2 + i%2);
        vote[i] = voter.getVoteAndProofs();
        
        KeyGen key_gen(ecg, gen, token_sums, id[i], 6);
        key_gen.setIDKey(id_key[i]);
        keys[i] = key_gen.getKeysAndProofs();
    }

    CryptoPP::ECPPoint vote_t[6];
    CryptoPP::ECPPoint key_t[6];
    for (int j = 0; j < 6; j++) {
        for (int i = 0; i < no_voters; i++) {
            vote_t[j] = ecg.curve.Add(vote_t[j], vote[i].value(j));
            key_t[j] = ecg.curve.Add(key_t[j], keys[i].value(j));
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
    assert(results[2] == 2);
    assert(results[3] == 2);
    assert(results[4] == 0);
    assert(results[5] == 0);
}