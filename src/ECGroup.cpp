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


void GenerateECGroup(CryptoPP::ECP& curve, 
                     CryptoPP::ECPPoint& base, 
                     CryptoPP::Integer& order)
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


CryptoPP::ECPPoint DecodeHexString(const std::string& hex_string, 
                                   const CryptoPP::ECP& curve)
{
    CryptoPP::HexDecoder decoder;
    decoder.Put((CryptoPP::byte*)hex_string.data(), hex_string.size());
    decoder.MessageEnd();

    std::string decoded;
    auto size = decoder.MaxRetrievable();
    if (size && size <= SIZE_MAX) {
        decoded.resize((size_t)size);	
        decoder.Get((CryptoPP::byte*)&decoded[0], decoded.size());
    }

    CryptoPP::ECPPoint point;
    curve.DecodePoint(point, (CryptoPP::byte*)decoded.data(), decoded.size());
    return point;       
}


CompressedPoint CompressPoint(const CryptoPP::ECPPoint& point, 
                          const CryptoPP::ECP& curve)
{
    CompressedPoint compressed;

    compressed.y = (point.y.IsOdd()) ? 1 : 0;
    compressed.x = point.x;

    return compressed;
}


CryptoPP::ECPPoint DecompressPoint(const CompressedPoint& compressed,
                                   const CryptoPP::ECP& curve)
{
    CryptoPP::Integer x = compressed.x;

    CryptoPP::ModularArithmetic field = curve.GetField();
    auto alpha1 = field.Multiply(x, field.Square(x));
    auto alpha2 = field.Multiply(x, CURVE_A);
    auto alpha = field.Add(alpha1, field.Add(alpha2, CURVE_B));

    auto p = field.GetModulus();
    auto beta = TonelliShanks(alpha, p);

    if (beta.IsOdd() != compressed.y)        
        return CryptoPP::ECPPoint(x, p - beta);
    return CryptoPP::ECPPoint(x, beta);
}


CryptoPP::Integer TonelliShanks(const CryptoPP::Integer& a, 
                                const CryptoPP::Integer& p) 
{
    CryptoPP::Integer s = 0;
    CryptoPP::Integer q = p - 1;
    while (q.IsEven()) {
        s += 1;
        q /= 2;
    }
    if (s == 1)
        return a_exp_b_mod_c(a, (p + 1)/4, p);

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
            if (i == m) {
                return 0;
            }            
        }
        CryptoPP::Integer b = a_exp_b_mod_c(c, 
                                            a_exp_b_mod_c(2, m - i - 1, p - 1), 
                                            p);
        m = i;
        c = a_times_b_mod_c(b, b, p);
        t = a_times_b_mod_c(t, c, p);
        r = a_times_b_mod_c(r, b, p);
    }
    
    return r;
}


CryptoPP::Integer RandomCoeff(const CryptoPP::ECP& curve)
{
    CryptoPP::byte block[32];
    CryptoPP::OS_GenerateRandomBlock(false, block, 32);
    CryptoPP::RandomPool rng;
    rng.IncorporateEntropy(block, 32);    

    return CryptoPP::Integer(rng, 
                             CryptoPP::Integer::Two(), 
                             curve.FieldSize() - 1);
}


CryptoPP::Integer RandomInteger(const CryptoPP::Integer& min, 
                                const CryptoPP::Integer& max)
{
    CryptoPP::byte block[32];
    CryptoPP::OS_GenerateRandomBlock(false, block, 32);
    CryptoPP::RandomPool rng;
    rng.IncorporateEntropy(block, 32);    

    return CryptoPP::Integer(rng, 
                             min, 
                             max);
}