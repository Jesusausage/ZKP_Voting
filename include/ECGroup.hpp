#ifndef EC_GROUP_HPP
#define EC_GROUP_HPP


#include <integer.h>
#include <ecp.h>
#include <hex.h>
#include <randpool.h>
#include <osrng.h>

#include <iostream>


struct CompressedPoint {
    CryptoPP::Integer x;
    bool y;
};


void GenerateECGroup(CryptoPP::ECP& curve, CryptoPP::ECPPoint& base);

CryptoPP::ECPPoint DecodeHexString(const std::string& hex_string, 
                                   const CryptoPP::ECP& curve);

CompressedPoint CompressPoint(const CryptoPP::ECPPoint& point, 
                          const CryptoPP::ECP& curve);

CryptoPP::ECPPoint DecompressPoint(const CompressedPoint& compressed,
                                   const CryptoPP::ECP& curve);

CryptoPP::Integer TonelliShanks(const CryptoPP::Integer& a, 
                                const CryptoPP::Integer& p);    

CryptoPP::Integer RandomCoeff(const CryptoPP::ECP& curve);                    


#endif