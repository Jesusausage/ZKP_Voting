#include "ECGroupTest.hpp"


void TestECGroupAddition()
{
    auto elliptic_curve = GenerateECGroup();

    CryptoPP::Integer g_x("86918276961810349294276103416548851884759982251107");
    CryptoPP::Integer g_y("87194829221142880348582938487511785107150118762739500766654458540580527283772");

    auto g = CryptoPP::ECPPoint(g_x, g_y);
    auto g_doubled = elliptic_curve.curve.Add(g, g);

    assert(g_doubled == elliptic_curve.base);
}


void TestDecodePoint()
{
    std::string hex_string = "0479BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8";
    
    auto elliptic_curve = GenerateECGroup();

    auto g = DecodeHexString(hex_string, elliptic_curve);
    assert(g == elliptic_curve.base);
}


void TestRandomPoint()
{
    auto elliptic_curve = GenerateECGroup();

    auto p = RandomInteger(1, elliptic_curve.order);
    auto q = RandomInteger(1, elliptic_curve.order);
    assert(p != q);

    auto P = elliptic_curve.curve.Multiply(p, elliptic_curve.base);
    auto Q = elliptic_curve.curve.Multiply(q, elliptic_curve.base);
    assert(P.x != Q.x);
    assert(P.y != Q.y);
}


void TestPointCompression()
{
    auto elliptic_curve = GenerateECGroup();

    CryptoPP::Integer p;
    CryptoPP::ECPPoint P;
    CompressedPoint P_comp;
    CryptoPP::ECPPoint P_decomp;

    for (int i = 0; i < 80; i++) {
        p = RandomInteger(1, elliptic_curve.order);
        P = elliptic_curve.curve.Multiply(p, elliptic_curve.base);

        P_comp = CompressPoint(P);
        P_decomp = DecompressPoint(P_comp, elliptic_curve);

        assert(P == P_decomp);
    }
}