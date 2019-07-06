#ifndef EC_GROUP_HPP
#define EC_GROUP_HPP


#include <integer.h>
#include <ecp.h>
#include <hex.h>


void GenerateECGroup(CryptoPP::ECP& curve, CryptoPP::ECPPoint& base);

CryptoPP::ECPPoint DecodeHexString(const std::string& hex_string, 
                                   const CryptoPP::ECP& curve);

std::string CompressPoint(const CryptoPP::ECPPoint& point, 
                          const CryptoPP::ECP& curve);


#endif