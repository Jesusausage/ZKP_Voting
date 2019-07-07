#ifndef EC_GROUP_HPP
#define EC_GROUP_HPP


#include <integer.h>
#include <ecp.h>
#include <hex.h>


struct CompPoint {
    CryptoPP::Integer x;
    bool y;
};


void GenerateECGroup(CryptoPP::ECP& curve, CryptoPP::ECPPoint& base);

CryptoPP::ECPPoint DecodeHexString(const std::string& hex_string, 
                                   const CryptoPP::ECP& curve);

CompPoint CompressPoint(const CryptoPP::ECPPoint& point, 
                          const CryptoPP::ECP& curve);

CryptoPP::ECPPoint DecompressPoint(const CompPoint& compressed,
                                   const CryptoPP::ECP& curve);

CryptoPP::Integer TonelliShanks(CryptoPP::Integer n, CryptoPP::Integer p);                         


#endif