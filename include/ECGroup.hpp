#ifndef EC_GROUP_HPP
#define EC_GROUP_HPP


#include <integer.h>
#include <ecp.h>
#include <hex.h>


class ECGroup {
public:
    ECGroup() 
    {
        auto power1 = CryptoPP::Integer::Power2(256);
        auto power2 = CryptoPP::Integer::Power2(32);
        CryptoPP::Integer p = power1 - power2 - 977;

        CryptoPP::Integer a = CryptoPP::Integer::Zero();
        CryptoPP::Integer b(7);

        curve = CryptoPP::ECP(p, a, b);

        base = newElement("0479BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8");
    }

    CryptoPP::ECPPoint newElement(std::string hex_string)
    {
        CryptoPP::HexDecoder decoder;
        decoder.Put((CryptoPP::byte*)hex_string.data(), hex_string.size());
        decoder.MessageEnd();

        std::string decoded;
        decoder.Get((CryptoPP::byte*)&decoded[0], decoded.size());

        CryptoPP::ECPPoint point;
        curve.DecodePoint(point, (CryptoPP::byte*)decoded.data(), decoded.size());       
    }

    CryptoPP::ECPPoint newElement(CryptoPP::Integer exp = 1) {
        return curve.ScalarMultiply(base, exp);
    }

private:
    CryptoPP::ECP curve;
    CryptoPP::ECPPoint base;
};



#endif