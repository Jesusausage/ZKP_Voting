#ifndef EC_GROUP_HPP
#define EC_GROUP_HPP


#include <cryptopp/integer.h>
#include <cryptopp/ecp.h>
#include <cryptopp/hex.h>
#include <cryptopp/randpool.h>
#include <cryptopp/osrng.h>
#include <fstream>

#include <iostream>


static const int CURVE_A = 0;
static const int CURVE_B = 7;
static const char BASE_X[] = "55066263022277343669578718895168534326250603453777594175500187360389116729240";
static const char BASE_Y[] = "32670510020758816978083085130507043184471273380659243275938904335757337482424";
static const char ORDER[] = "115792089237316195423570985008687907852837564279074904382605163141518161494337";
const int FIELD_SIZE = 32;


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
                                size_t size = 32);

void WriteID(const CryptoPP::ECPPoint& id,
             std::ostream& o);

void WriteTokens(const CryptoPP::ECPPoint* tokens,
                 const int num_options,
                 std::ostream& o);
void WriteTokens(const std::vector<CryptoPP::ECPPoint> tokens,
                 std::ostream& o);

void ReadIDs(std::istream& i,
             CryptoPP::ECPPoint* ids);

void ReadTokens(std::istream& i,
                CryptoPP::ECPPoint** tokens,
                const int num_options);


#endif