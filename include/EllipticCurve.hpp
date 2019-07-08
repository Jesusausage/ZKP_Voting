#ifndef ELLIPTIC_CURVE_HPP
#define ELLIPTIC_CURVE_HPP


#include <ecp.h>


const int CURVE_A = 0;
const int CURVE_B = 7;


class EllipticCurve {
public:
    EllipticCurve();

    

private:
    CryptoPP::ECP _curve;
    CryptoPP::ECPPoint _base;
    CryptoPP::Integer _order;
}


#endif