#include "ParamGen.hpp"


void generateParams() 
{
    auto ecg = GenerateECGroup();
    auto base = GenerateECBase();

    std::ofstream token_out(TOKEN_FILE);
    std::ofstream id_out(ID_FILE);

    CryptoPP::Integer token_keys[10][5];
    CryptoPP::ECPPoint tokens[10][5];
    CryptoPP::ECPPoint token_sums[5];

    CryptoPP::Integer id_keys[10];
    CryptoPP::ECPPoint ids[10];
    CryptoPP::ECPPoint id_sum;

    for (int i = 0; i < 10; i++) {
        for (int option = 0; option < 5; option++) {
            token_keys[i][option] = RandomInteger(1, ecg.order);
            tokens[i][option] = ecg.curve.Multiply(token_keys[i][option], base);
            token_sums[option] = ecg.curve.Add(token_sums[option], tokens[i][option]);
        }
        WriteTokens(tokens[i], 5, token_out);
        id_keys[i] = RandomInteger(1, ecg.order);
        ids[i] = ecg.curve.Multiply(id_keys[i], base);
        id_sum = ecg.curve.Add(id_sum, ids[i]);
        WriteID(ids[i], id_out);
    }
    token_out.close();
    id_out.close();

    std::ofstream priv_out("private_keys.txt");
    priv_out << id_keys[0] << std::endl;
    for (int i = 0; i < 5; ++i)
        priv_out << token_keys[0][i] << std::endl;
    priv_out.close();
    
    priv_out.open("private_keys1.txt");
    priv_out << id_keys[1] << std::endl;
    for (int i = 0; i < 5; ++i)
        priv_out << token_keys[1][i] << std::endl;
    priv_out.close();



    PublicData pub(ecg, 10, 5);
    PrivateData priv(5);
    VoteData data(ecg, base, pub, priv);

    for (int i = 1; i < 10; ++i) {
        Voter voter(ecg, base, id_sum, tokens[i], 5);
        voter.setTokenKeys(token_keys[i]);
        voter.castVote(i % 5);
        Vote vote = voter.getVoteAndProofs();

        KeyGen key_gen(ecg, base, token_sums, ids[i], 5);
        key_gen.setIDKey(id_keys[i]);
        Key key = key_gen.getKeysAndProofs();

        CryptoPP::byte output[2445];
        int n;
        vote.serialise(output, n);
        key.serialise(output + 1630, n);

        data.processVKPair(output, i);
    }
}