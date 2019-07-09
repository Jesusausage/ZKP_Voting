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

struct ECGroup {
    CryptoPP::ECP curve;
    CryptoPP::ECPPoint base;
    CryptoPP::Integer order;
};


ECGroup GenerateECGroup();

CryptoPP::ECPPoint DecodeHexString(const std::string& hex_string, 
                                   const ECGroup& ec);

CompressedPoint CompressPoint(const CryptoPP::ECPPoint& point);

CryptoPP::ECPPoint DecompressPoint(const CompressedPoint& compressed,
                                   const ECGroup& ec);

CryptoPP::Integer TonelliShanks(const CryptoPP::Integer& a, 
                                const CryptoPP::Integer& p);    

CryptoPP::Integer RandomInteger(const CryptoPP::Integer& min, 
                                const CryptoPP::Integer& max, 
                                CryptoPP::byte* seed = nullptr, 
                                size_t size = 0);


#endif