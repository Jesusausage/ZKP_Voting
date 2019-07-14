#ifndef EC_GROUP_HPP
#define EC_GROUP_HPP


#include <cryptopp/integer.h>
#include <cryptopp/ecp.h>
#include <cryptopp/hex.h>
#include <cryptopp/randpool.h>
#include <cryptopp/osrng.h>

#include <iostream>


struct ECGroup {
    CryptoPP::ECP curve;
    CryptoPP::ECPPoint base;
    CryptoPP::Integer order;
};

struct CompressedPoint {
    CryptoPP::Integer x;
    bool y;
};


ECGroup GenerateECGroup();

CryptoPP::ECPPoint DecodeHexString(const std::string& hex_string, 
                                   const CryptoPP::ECP& ec);

CompressedPoint CompressPoint(const CryptoPP::ECPPoint& point);

CryptoPP::ECPPoint DecompressPoint(const CompressedPoint& compressed,
                                   const CryptoPP::ECP& ec);

CryptoPP::Integer TonelliShanks(const CryptoPP::Integer& a, 
                                const CryptoPP::Integer& p);    

CryptoPP::Integer RandomInteger(const CryptoPP::Integer& min, 
                                const CryptoPP::Integer& max, 
                                CryptoPP::byte* seed = nullptr, 
                                size_t size = 0);


#endif