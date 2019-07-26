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
    std::string id_file("IDs.txt");

    std::ofstream id_out(id_file);
    CryptoPP::ECPPoint ids[10];
    for (int i = 0; i < 10; i++) {
        ids[i] = ecg.curve.Multiply(RandomInteger(1, ecg.order), ecg.base);
        WriteID(ids[i], id_out);
    }
    id_out.close();

    VoteData data(10, 3);
    data.readIDsFromFile(id_file);
    for (int i = 0; i < 10; i++) {
        assert(data.voterID(i) == ids[i]);
    }
}


void TestReadIDs()
{
    auto ecg = GenerateECGroup();
    std::string token_file("tokens.txt");

    std::ofstream token_out(token_file);
    CryptoPP::ECPPoint tokens[10][5];
    for (int i = 0; i < 10; i++) {
        for (int option = 0; option < 5; option++) {
            tokens[i][option] = ecg.curve.Multiply(RandomInteger(1, ecg.order), ecg.base);
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