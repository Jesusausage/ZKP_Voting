#include "ECGroupTest.hpp"


void TestECGroupAddition()
{
    CryptoPP::ECP curve;
    CryptoPP::ECPPoint base;
    GenerateECGroup(curve, base);

    CryptoPP::Integer g_x("86918276961810349294276103416548851884759982251107");
    CryptoPP::Integer g_y("87194829221142880348582938487511785107150118762739500766654458540580527283772");

    auto g = CryptoPP::ECPPoint(g_x, g_y);
    auto g_doubled = curve.Add(g, g);

    assert(g_doubled == base);
}


void TestDecodePoint()
{
    std::string hex_string = "0479BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8";
    
    CryptoPP::ECP curve;
    CryptoPP::ECPPoint base;
    GenerateECGroup(curve, base);

    auto g = DecodeHexString(hex_string, curve);
    assert(g == base);
}


void TestRandomPoint()
{
    CryptoPP::ECP curve;
    CryptoPP::ECPPoint base;
    GenerateECGroup(curve, base);

    auto p = RandomCoeff(curve);
    auto q = RandomCoeff(curve);
    assert(p != q);

    auto P = curve.Multiply(p, base);
    auto Q = curve.Multiply(q, base);
    assert(P.x != Q.x);
    assert(P.y != Q.y);
}


void TestPointCompression()
{
    CryptoPP::ECP curve;
    CryptoPP::ECPPoint base;
    GenerateECGroup(curve, base);

    CryptoPP::Integer p;
    CryptoPP::ECPPoint P;
    CompressedPoint P_comp;
    CryptoPP::ECPPoint P_decomp;

    for (int i = 0; i < 80; i++) {
        p = RandomCoeff(curve);
        P = curve.Multiply(p, base);

        P_comp = CompressPoint(P, curve);
        P_decomp = DecompressPoint(P_comp, curve);

        assert(P == P_decomp);
    }
}