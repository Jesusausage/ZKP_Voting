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
    Key keys = key_gen.getKeysAndProofs();

    // keys.values[8].x += 1;
    
    for (int i = 0; i < 10; i++) {
        ElGamalProtocol prot(ecg, gen, 0);
        prot.setParams(token_sums[i], id, keys.values[i]);

        assert(prot.verifyNIZKP(keys.proofs[i]) == true);
    }
}