#include "KeyGenTest.hpp"


void TestKeyGeneration()
{
    auto ecg = GenerateECGroup();

    auto gen = ecg.base;

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

    CryptoPP::byte o[1630];
    int n;
    key.serialise(o, n);
    Key ey(o, n, ecg.curve);

    // for (int i = 0; i < 10; i++) {
    //     std::cout << i << std::endl;
    //     assert(key.value(i).x == ey.value(i).x);
    //     assert(key.value(i).y == ey.value(i).y);

    //     assert(key.proof(i).commitment()->x == ey.proof(i).commitment()->x);
    //     assert(key.proof(i).commitment()->y == ey.proof(i).commitment()->y);

    //     std::cout << (key.proof(i).commitment() + 1)->y << std::endl;
    //     std::cout << (ey.proof(i).commitment() + 1)->y << std::endl;
    //     assert((key.proof(i).commitment() + 1)->x == (ey.proof(i).commitment() + 1)->x);
    //     assert((key.proof(i).commitment() + 1)->y == (ey.proof(i).commitment() + 1)->y);

    //     assert(key.proof(i).challenge() == ey.proof(i).challenge());
    //     assert(key.proof(i).response() == ey.proof(i).response());
    //     assert(key.proof(i).commitmentSize() == ey.proof(i).commitmentSize());
    // }
    
    for (int i = 0; i < 10; i++) {
        ElGamalProtocol prot(ecg, gen, 0);
        prot.setParams(token_sums[i], id, ey.value(i));
        assert(prot.verifyNIZKP(ey.proof(i)) == true);
    }
}