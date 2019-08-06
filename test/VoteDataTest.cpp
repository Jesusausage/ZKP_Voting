#include "VoteDataTest.hpp"


void VoteDataTest::testReadOptionsAndIPs()
{
    auto ecg = GenerateECGroup();
    auto base = GenerateECBase();

    std::ofstream opts(OPTION_FILE);
    opts << "Celery" << std::endl 
         << "Carrot" << std::endl 
         << "Onion" << std::endl
         << "Tomato" << std::endl
         << "Potato" << std::endl;
    opts.close();
    std::ofstream ips(IP_FILE);
    ips << "292.168.27.1" << std::endl 
        << "192.168.27.2" << std::endl 
        << "192.168.27.3" << std::endl
        << "192.168.27.4" << std::endl
        << "192.168.27.5" << std::endl
        << "192.168.27.2" << std::endl 
        << "192.168.27.3" << std::endl
        << "192.168.27.4" << std::endl
        << "192.168.27.5" << std::endl
        << "192.168.27.1" << std::endl;
    ips.close();

    VoteData data(ecg, base, 10, 5);

    assert(data.options_[0] == "Celery");
    assert(data.options_[1] == "Carrot");
    assert(data.options_[2] == "Onion");
    assert(data.options_[3] == "Tomato");
    assert(data.options_[4] == "Potato");

    assert(data.ip_addrs_[0] == "292.168.27.1");
    assert(data.ip_addrs_[1] == "192.168.27.2");
    assert(data.ip_addrs_[2] == "192.168.27.3");
    assert(data.ip_addrs_[3] == "192.168.27.4");
    assert(data.ip_addrs_[4] == "192.168.27.5");
    assert(data.ip_addrs_[9] == "192.168.27.1");
}


void VoteDataTest::testReadTokensAndIDs()
{
    auto ecg = GenerateECGroup();
    auto base = GenerateECBase();

    std::ofstream token_out(TOKEN_FILE);
    std::ofstream id_out(ID_FILE);
    CryptoPP::ECPPoint tokens[10][5];
    CryptoPP::ECPPoint ids[10];
    for (int i = 0; i < 10; i++) {
        for (int option = 0; option < 5; option++) {
            tokens[i][option] = ecg.curve.Multiply(RandomInteger(1, ecg.order), base);
        }
        WriteTokens(tokens[i], 5, token_out);
        ids[i] = ecg.curve.Multiply(RandomInteger(1, ecg.order), base);
        WriteID(ids[i], id_out);
    }
    token_out.close();
    id_out.close();

    VoteData data(ecg, base, 10, 5);

    for (int i = 0; i < 10; i++) {
        for (int option = 0; option < 5; option++) {
            assert(data.tokens_[i][option] == tokens[i][option]);
        }
        assert(data.voter_ids_[i] == ids[i]);
    }
}


void VoteDataTest::testWriteVote()
{
    auto ecg = GenerateECGroup();
    auto gen = GenerateECBase();

    auto id_sum = ecg.curve.Multiply(27, gen);
    std::vector<CryptoPP::Integer> token_keys;
    std::vector<CryptoPP::ECPPoint> tokens;
    for (int i = 0; i < 5; i++) {
        token_keys.push_back(RandomInteger(1, ecg.order));
        tokens.push_back(ecg.curve.Multiply(token_keys[i], gen));
    }

    Voter voter(ecg, gen, id_sum, tokens);
    voter.setTokenKeys(token_keys);
    voter.castVote(2);
    Vote vote = voter.getVoteAndProofs();

    VoteData data(ecg, gen, 10, 5);
    data.writeVote(vote, 0);

    CryptoPP::byte output[1630];
    VoteData::readVote(0, output, 5);
    Vote readvote(output, 5, ecg.curve);

    for (int i = 0; i < 5; i++) {
        ElGamalProtocol prot0(ecg, gen, 0);
        ElGamalProtocol prot1(ecg, gen, 1);
        prot0.setParams(id_sum, tokens[i], readvote.value(i));
        prot1.setParams(id_sum, tokens[i], readvote.value(i));

        SigmaProtocol* prots[2] = {&prot0, &prot1};
        OrProtocol prot(prots, 2);
        assert(prot.verifyNIZKP(readvote.proof(i)) == true);
    }
}


void VoteDataTest::testWriteKey()
{
    auto ecg = GenerateECGroup();
    auto gen = GenerateECBase();

    auto id_key = RandomInteger(2, ecg.order);
    auto id = ecg.curve.Multiply(id_key, gen);
    std::vector<CryptoPP::ECPPoint> token_sums;
    for (int i = 0; i < 5; i++) {
        auto x = RandomInteger(2, ecg.order);
        token_sums.push_back(ecg.curve.Multiply(x, gen));
    }

    KeyGen key_gen(ecg, gen, token_sums, id);
    key_gen.setIDKey(id_key);
    Key key = key_gen.getKeysAndProofs();

    VoteData data(ecg, gen, 10, 5);
    data.writeKey(key, 0);
    
    CryptoPP::byte output[815];
    VoteData::readKey(0, output, 5);
    Key readkey(output, 5, ecg.curve);

    for (int i = 0; i < 5; i++) {
        ElGamalProtocol prot(ecg, gen, 0);
        prot.setParams(token_sums[i], id, readkey.value(i));
        assert(prot.verifyNIZKP(readkey.proof(i)) == true);
    }
}


void VoteDataTest::testProcessHashes()
{
    auto ecg = GenerateECGroup();
    auto gen = GenerateECBase();

    auto id_key = RandomInteger(2, ecg.order);
    auto id = ecg.curve.Multiply(id_key, gen);
    auto id_sum = id;

    std::vector<CryptoPP::Integer> token_keys;
    std::vector<CryptoPP::ECPPoint> tokens;
    for (int i = 0; i < 5; i++) {
        token_keys.push_back(RandomInteger(2, ecg.order));
        tokens.push_back(ecg.curve.Multiply(token_keys[i], gen));
    }
    std::vector<CryptoPP::ECPPoint> token_sums = tokens;

    Voter voter(ecg, gen, id_sum, tokens);
    voter.setTokenKeys(token_keys);
    voter.castVote(1);
    Vote vote = voter.getVoteAndProofs();

    KeyGen key_gen(ecg, gen, token_sums, id);
    key_gen.setIDKey(id_key);
    Key key = key_gen.getKeysAndProofs();

    CryptoPP::byte output[2445];
    int n;
    vote.serialise(output, n);
    key.serialise(output + 1630, n);

    VoteData data(ecg, gen, 10, 5);
    data.processVKPair(output, 0);
    assert(data.received_[0] == false);
}


void VoteDataTest::testSuccessfulVote()
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

    VoteData data(ecg, base, 10, 5);

    Voter voter(ecg, base, id_sum, tokens[1], 5);
    voter.setTokenKeys(token_keys[1]);
    voter.castVote(1);
    Vote vote = voter.getVoteAndProofs();

    KeyGen key_gen(ecg, base, token_sums, ids[1], 5);
    key_gen.setIDKey(id_keys[1]);
    Key key = key_gen.getKeysAndProofs();

    CryptoPP::byte output[2445];
    int n;
    vote.serialise(output, n);
    key.serialise(output + 1630, n);

    data.processVKPair(output, 1);
    assert(data.received_[1] == true);
}


void VoteDataTest::testGetUserVote()
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

    std::ofstream priv_out(PRIV_KEY_FILE);
    priv_out << id_keys[3] << std::endl;
    for (int i = 0; i < 5; ++i)
        priv_out << token_keys[3][i] << std::endl;
    priv_out.close();

    VoteData data(ecg, base, 10, 5);
    assert(data.received_[3] == true);
}