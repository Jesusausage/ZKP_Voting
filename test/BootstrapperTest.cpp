#include "BootstrapperTest.hpp"


void TestReadOptions()
{
    std::string filename = "options.txt";

    std::ofstream opts(filename);
    opts << "Celery" << std::endl 
         << "Carrot" << std::endl 
         << "Onion" << std::endl;
    opts.close();

    Bootstrapper bs(10, 3);
    bs.readOptionsFromFile(filename);
    assert(bs.option(0) == "Celery");
    assert(bs.option(1) == "Carrot");
    assert(bs.option(2) == "Onion");
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

    Bootstrapper bs(10, 3);
    bs.readIDsFromFile(id_file);
    for (int i = 0; i < 10; i++) {
        assert(bs.voterID(i) == ids[i]);
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

    Bootstrapper bs(10, 5);
    bs.readTokensFromFile(token_file);

    for (int i = 0; i < 10; i++) {
        for (int option = 0; option < 5; option++) {
            assert(bs.token(i, option) == tokens[i][option]);
        }
    }
}