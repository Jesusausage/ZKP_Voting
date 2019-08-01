#include "VoteDataTest.hpp"


void TestReadOptions()
{
    std::string filename = "options.txt";

    std::ofstream opts(filename);
    opts << "Celery" << std::endl 
         << "Carrot" << std::endl 
         << "Onion" << std::endl;
    opts.close();

    VoteData data(10, 3);
    data.readOptionsFromFile(filename);
    assert(data.option(0) == "Celery");
    assert(data.option(1) == "Carrot");
    assert(data.option(2) == "Onion");
}


void TestReadTokens()
{
    auto ecg = GenerateECGroup();
    auto base = GenerateECBase();
    std::string token_file("tokens.txt");

    std::ofstream token_out(token_file);
    CryptoPP::ECPPoint tokens[10][5];
    for (int i = 0; i < 10; i++) {
        for (int option = 0; option < 5; option++) {
            tokens[i][option] = ecg.curve.Multiply(RandomInteger(1, ecg.order), base);
        }
        WriteTokens(tokens[i], 5, token_out);
    }
    token_out.close();

    VoteData data(10, 5);
    data.readTokensFromFile(token_file);

    for (int i = 0; i < 10; i++) {
        for (int option = 0; option < 5; option++) {
            assert(data.token(i, option) == tokens[i][option]);
        }
    }
}


void TestReadIDs()
{
    auto ecg = GenerateECGroup();
    auto base = GenerateECBase();
    std::string id_file("IDs.txt");

    std::ofstream id_out(id_file);
    CryptoPP::ECPPoint ids[10];
    for (int i = 0; i < 10; i++) {
        ids[i] = ecg.curve.Multiply(RandomInteger(1, ecg.order), base);
        WriteID(ids[i], id_out);
    }
    id_out.close();

    VoteData data(10, 3);
    data.readIDsFromFile(id_file);
    for (int i = 0; i < 10; i++) {
        assert(data.voterID(i) == ids[i]);
    }
}


void TestReadIPs()
{    
    std::string filename = "IPs.txt";

    std::ofstream opts(filename);
    opts << "192.168.27.1" << std::endl 
         << "192.168.26.1" << std::endl 
         << "192.168.27.193" << std::endl;
    opts.close();

    VoteData data(3, 3);
    data.readIPsFromFile(filename);
    assert(data.ip(0) == "192.168.27.1");
    assert(data.ip(1) == "192.168.26.1");
    assert(data.ip(2) == "192.168.27.193");
}


void VoteDataTest::testWriteVote()
{
    auto ecg = GenerateECGroup();
    auto gen = GenerateECBase();
    auto id_sum = ecg.curve.Multiply(27, gen);
    std::vector<CryptoPP::Integer> token_keys;
    std::vector<CryptoPP::ECPPoint> tokens;
    for (int i = 0; i < 10; i++) {
        token_keys.push_back(RandomInteger(1, ecg.order));
        tokens.push_back(ecg.curve.Multiply(token_keys[i], gen));
    }

    Voter voter(ecg, gen, id_sum, tokens);
    voter.setTokenKeys(token_keys);
    voter.castVote(8);
    Vote vote = voter.getVoteAndProofs();

    VoteData data(3, 10);
    data.writeVote(vote, 0);
}


void VoteDataTest::testWriteKey()
{
    auto ecg = GenerateECGroup();
    auto gen = GenerateECBase();
    auto id_key = RandomInteger(2, ecg.order);
    auto id = ecg.curve.Multiply(id_key, gen);
    std::vector<CryptoPP::ECPPoint> token_sums;
    for (int i = 0; i < 10; i++) {
        auto x = RandomInteger(2, ecg.order);
        token_sums.push_back(ecg.curve.Multiply(x, gen));
    }

    KeyGen key_gen(ecg, gen, token_sums, id);
    key_gen.setIDKey(id_key);
    Key key = key_gen.getKeysAndProofs();

    VoteData data(3, 10);
    data.writeKey(key, 0);
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
    for (int i = 0; i < 3; i++) {
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

    std::string hash_data = vote.getHashData();
    hash_data += key.getHashData();

    CryptoPP::byte** hash = new CryptoPP::byte*[1];
    hash[0] = new CryptoPP::byte[32];
    HashTo32(hash_data, hash[0]);

    VoteData data(1, 3);
    data.processHashes(hash, 0);

    assert(data.badHash(hash[0]));
}