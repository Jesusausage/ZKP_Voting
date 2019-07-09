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

    auto g = DecodeHexString(hex_string, ecg);
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
        P_decomp = DecompressPoint(P_comp, ecg);

        assert(P == P_decomp);
    }
}