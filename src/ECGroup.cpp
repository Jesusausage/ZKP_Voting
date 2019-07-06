#include "ECGroup.hpp"


const int CURVE_A = 0;
const int CURVE_B = 7;


void GenerateECGroup(CryptoPP::ECP& curve, CryptoPP::ECPPoint& base)
{
        auto power1 = CryptoPP::Integer::Power2(256);
        auto power2 = CryptoPP::Integer::Power2(32);
        CryptoPP::Integer p = power1 - power2 - 977;

        curve = CryptoPP::ECP(p, CURVE_A, CURVE_B);

        CryptoPP::Integer base_x("55066263022277343669578718895168534326250603453777594175500187360389116729240");
        CryptoPP::Integer base_y("32670510020758816978083085130507043184471273380659243275938904335757337482424");
        base = CryptoPP::ECPPoint(base_x, base_y);
}


CryptoPP::ECPPoint DecodeHexString(const std::string& hex_string, 
                                   const CryptoPP::ECP& curve)
{
    CryptoPP::HexDecoder decoder;
    decoder.Put((CryptoPP::byte*)hex_string.data(), hex_string.size());
    decoder.MessageEnd();

    std::string decoded;
    CryptoPP::word64 size = decoder.MaxRetrievable();
    if (size && size <= SIZE_MAX) {
        decoded.resize(size);		
        decoder.Get((CryptoPP::byte*)&decoded[0], decoded.size());
    }

    CryptoPP::ECPPoint point;
    curve.DecodePoint(point, (CryptoPP::byte*)decoded.data(), decoded.size());
    return point;       
}


std::string CompressPoint(const CryptoPP::ECPPoint& point, 
                          const CryptoPP::ECP& curve)
{
    std::string compressed;

    compressed += (point.y.IsOdd()) ? "1" : "0";
    compressed += CryptoPP::IntToString<CryptoPP::Integer>(point.x);

    return compressed;
}


// CryptoPP::ECPPoint DecompressPoint(const std::string& compressed,
//                                    const CryptoPP::ECP& curve)
// {
//     CryptoPP::Integer y(compressed[0]);
//     CryptoPP::Integer x(compressed.substr(1).c_str());

//     CryptoPP::ModularArithmetic field = curve.GetField();
//     auto alpha1 = field.Multiply(x, field.Square(x));
//     auto alpha2 = field.Multiply(x, CURVE_A);
//     auto alpha = field.Add(alpha1, field.Add(alpha2, CURVE_B));
// }


// long pow_mod(long x, long n, long p) {
//   if (n == 0) return 1;
//   if (n & 1)
//     return (pow_mod(x, n-1, p) * x) % p;
//   x = pow_mod(x, n/2, p);
//   return (x * x) % p;
// }

// /* Takes as input an odd prime p and n < p and returns r
//  * such that r * r = n [mod p]. */
// long tonelli_shanks(long n, long p) {
//   long s = 0;
//   long q = p - 1;
//   while ((q & 1) == 0) { q /= 2; ++s; }
//   if (s == 1) {
//     long r = pow_mod(n, (p+1)/4, p);
//     if ((r * r) % p == n) return r;
//     return 0;
//   }
//   // Find the first quadratic non-residue z by brute-force search
//   long z = 1;
//   while (pow_mod(++z, (p-1)/2, p) != p - 1);
//   long c = pow_mod(z, q, p);
//   long r = pow_mod(n, (q+1)/2, p);
//   long t = pow_mod(n, q, p);
//   long m = s;
//   while (t != 1) {
//     long tt = t;
//     long i = 0;
//     while (tt != 1) {
//       tt = (tt * tt) % p;
//       ++i;
//       if (i == m) return 0;
//     }
//     long b = pow_mod(c, pow_mod(2, m-i-1, p-1), p);
//     long b2 = (b * b) % p;
//     r = (r * b) % p;
//     t = (t * b2) % p;
//     c = b2;
//     m = i;
//   }
//   if ((r * r) % p == n) return r;
//   return 0;
// }