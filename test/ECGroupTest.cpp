#include "ECGroupTest.hpp"


void TestECGroupAddition()
{
    ECGroup curve;

    CryptoPP::Integer g_x("86918276961810349294276103416548851884759982251107");
    CryptoPP::Integer g_y("87194829221142880348582938487511785107150118762739500766654458540580527283772");

    auto g = curve.newElement(g_x, g_y);
    auto g_doubled = curve.add(g, g);

    assert(g_doubled == curve.base());
}