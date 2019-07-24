#include "ECGroupTest.hpp"


void TestECGroupAddition()
{
    auto ecg = GenerateECGroup();

    CryptoPP::Integer g_x("86918276961810349294276103416548851884759982251107");
    CryptoPP::Integer g_y("87194829221142880348582938487511785107150118762739500766654458540580527283772");

    auto g = CryptoPP::ECPPoint(g_x, g_y);
    auto g_doubled = ecg.curve.Add(g, g);

    assert(g_doubled == ecg.base);
}


void TestDecodePoint()
{
    std::string hex_string = "0479BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8";
    
    auto ecg = GenerateECGroup();

    auto g = DecodeHexString(hex_string, ecg.curve);
    assert(g == ecg.base);
}


void TestRandomPoint()
{
    auto ecg = GenerateECGroup();

    auto p = RandomInteger(1, ecg.order);
    auto q = RandomInteger(1, ecg.order);
    assert(p != q);

    auto P = ecg.curve.Multiply(p, ecg.base);
    auto Q = ecg.curve.Multiply(q, ecg.base);
    assert(P.x != Q.x);
    assert(P.y != Q.y);
}


void TestPointCompression()
{
    auto ecg = GenerateECGroup();

    CryptoPP::Integer p;
    CryptoPP::ECPPoint P;
    CompressedPoint P_comp;
    CryptoPP::ECPPoint P_decomp;

    for (int i = 0; i < 80; i++) {
        p = RandomInteger(1, ecg.order);
        P = ecg.curve.Multiply(p, ecg.base);

        P_comp = CompressPoint(P);
        P_decomp = DecompressPoint(P_comp, ecg.curve);

        assert(P == P_decomp);
    }
}


void TestWrite()
{
    auto ecg = GenerateECGroup();
    std::string id_file("IDs.txt");
    std::string token_file("tokens.txt");

    std::ofstream id_out(id_file);
    CryptoPP::ECPPoint ids[10];
    for (int i = 0; i < 10; i++) {
        ids[i] = ecg.curve.Multiply(RandomInteger(1, ecg.order), ecg.base);
        WriteID(ids[i], id_out);
    }
    id_out.close();

    std::ofstream token_out(token_file);
    CryptoPP::ECPPoint tokens[10][5];
    for (int i = 0; i < 10; i++) {
        for (int option = 0; option < 5; option++) {
            tokens[i][option] = ecg.curve.Multiply(RandomInteger(1, ecg.order), ecg.base);
        }
        WriteTokens(tokens[i], 5, token_out);
    }
    token_out.close();

    std::ifstream id_in(id_file);
    CryptoPP::ECPPoint read_ids[10];
    ReadIDs(id_in, read_ids);
    for (int i = 0; i < 10; i++) {
        assert(read_ids[i] == ids[i]);
    }
    id_in.close();

    std::ifstream token_in(token_file);
    CryptoPP::ECPPoint* read_tokens[10]; 
    for (int i = 0; i < 10; i++)
        read_tokens[i] = new CryptoPP::ECPPoint[5];
    ReadTokens(token_in, read_tokens, 5);
    for (int i = 0; i < 10; i++) {
        for (int option = 0; option < 5; option++) {
            assert(read_tokens[i][option] == tokens[i][option]);
        }
    }
    token_in.close();
    for (int i = 0; i < 10; i++)
        delete [] read_tokens[i];
}