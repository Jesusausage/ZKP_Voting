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



CompPoint CompressPoint(const CryptoPP::ECPPoint& point, 
                          const CryptoPP::ECP& curve)
{
    CompPoint compressed;

    compressed.y = (point.y.IsOdd()) ? 1 : 0;
    compressed.x = point.x;

    return compressed;
}


CryptoPP::ECPPoint DecompressPoint(const CompPoint& compressed,
                                   const CryptoPP::ECP& curve)
{
    CryptoPP::Integer x = compressed.x;

    CryptoPP::ModularArithmetic field = curve.GetField();
    auto alpha1 = field.Multiply(x, field.Square(x));
    auto alpha2 = field.Multiply(x, CURVE_A);
    auto alpha = field.Add(alpha1, field.Add(alpha2, CURVE_B));

    auto beta = TonelliShanks(alpha, field.GetModulus());
    if (beta.IsOdd() != compressed.y)
        beta.Negate();
    return CryptoPP::ECPPoint(x, beta);
}


CryptoPP::Integer TonelliShanks(CryptoPP::Integer a, CryptoPP::Integer p) 
{
    CryptoPP::Integer s = 0;
    CryptoPP::Integer q = p - 1;
    while (q.IsEven()) {
        s += 1;
        q /= 2;
    }

    CryptoPP::Integer z = 1;
    while (a_exp_b_mod_c(z, (p - 1)/2, p) != p - 1)
        z++;

    CryptoPP::Integer m = s;
    CryptoPP::Integer c = a_exp_b_mod_c(z, q, p);
    CryptoPP::Integer t = a_exp_b_mod_c(a, q, p);
    CryptoPP::Integer r = a_exp_b_mod_c(a, (q + 1)/2, p);
    while (t != 1) {
        CryptoPP::Integer i = 1;
        while (a_exp_b_mod_c(t, 2*i, p) != 1) {
            i++;
            if (i == m)
                return 0;            
        }

        CryptoPP::Integer b = a_exp_b_mod_c(c, 
                                            a_exp_b_mod_c(2, m - i - 1, p - 1), 
                                            p);
        m = i;
        c = a_times_b_mod_c(b, b, p);
        t = a_times_b_mod_c(t, c, p);
        r = a_times_b_mod_c(r, b, p);
    }

    if (a_times_b_mod_c(r, r, p) == a) 
        return r;

    return 0;
}