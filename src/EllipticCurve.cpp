#include "EllipticCurve.hpp"


EllipticCurve::EllipticCurve()
{
    auto power1 = CryptoPP::Integer::Power2(256);
    auto power2 = CryptoPP::Integer::Power2(32);
    CryptoPP::Integer p = power1 - power2 - 977;

    curve = CryptoPP::ECP(p, CURVE_A, CURVE_B);

    CryptoPP::Integer base_x("55066263022277343669578718895168534326250603453777594175500187360389116729240");
    CryptoPP::Integer base_y("32670510020758816978083085130507043184471273380659243275938904335757337482424");
    base = CryptoPP::ECPPoint(base_x, base_y);
    order = CryptoPP::Integer("115792089237316195423570985008687907852837564279074904382605163141518161494337");
}