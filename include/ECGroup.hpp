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

        _curve = CryptoPP::ECP(p, a, b);

        CryptoPP::Integer base_x("55066263022277343669578718895168534326250603453777594175500187360389116729240");
        CryptoPP::Integer base_y("32670510020758816978083085130507043184471273380659243275938904335757337482424");
        _base = CryptoPP::ECPPoint(base_x, base_y);
    }

    CryptoPP::ECPPoint newElement(std::string hex_string)
    {
        CryptoPP::HexDecoder decoder;
        decoder.Put((CryptoPP::byte*)hex_string.data(), hex_string.size());
        decoder.MessageEnd();

        std::string decoded;
        decoder.Get((CryptoPP::byte*)&decoded[0], decoded.size());

        CryptoPP::ECPPoint point;
        _curve.DecodePoint(point, (CryptoPP::byte*)decoded.data(), decoded.size());       
    }

    CryptoPP::ECPPoint newElement(CryptoPP::Integer x, CryptoPP::Integer y)
    {
        return CryptoPP::ECPPoint(x, y);
    }

    CryptoPP::ECPPoint newElement(const CryptoPP::Integer& exp = 1) 
    {
        return _curve.ScalarMultiply(_base, exp);
    }

    CryptoPP::ECPPoint add(const CryptoPP::ECPPoint& x, const CryptoPP::ECPPoint& y)
    {
        return _curve.Add(x, y);
    }

    CryptoPP::ECPPoint base()
    {
        return _base;
    }

private:
    CryptoPP::ECP _curve;
    CryptoPP::ECPPoint _base;
};



#endif