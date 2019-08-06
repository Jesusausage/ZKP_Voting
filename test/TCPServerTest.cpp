#include "TCPServerTest.hpp"


void TestIntByteConversion()
{
    int a = 67;
    CryptoPP::byte ch[4];
    IntToByte(a, ch);
    assert(ByteToInt(ch) == a);

    a = 67664;
    IntToByte(a, ch);
    assert(ByteToInt(ch) == a);

    a = 672;
    IntToByte(a, ch);
    assert(ByteToInt(ch) == a);

    a = -1;
    IntToByte(a, ch);
    assert(ByteToInt(ch) == a);
}