#include "Key.hpp"


Key::Key(CryptoPP::ECPPoint* values, Transcript* proofs, int num_options)
         : 
         num_options_(num_options)
{
    values_ = new CryptoPP::ECPPoint[num_options_];
    proofs_ = new Transcript[num_options_];
    for (int i = 0; i < num_options_; i++) {
        values_[i] = values[i];
        proofs_[i] = proofs[i];
    }
}


Key::Key(int num_options) : num_options_(num_options)
{
    values_ = new CryptoPP::ECPPoint[num_options_];
    proofs_ = new Transcript[num_options_];
}


Key::Key(const Key& key) : num_options_(key.num_options_)
{
    values_ = new CryptoPP::ECPPoint[num_options_];
    proofs_ = new Transcript[num_options_];
    for (int i = 0; i < num_options_; i++) {
        values_[i] = key.values_[i];
        proofs_[i] = key.proofs_[i];
    }
}


Key::Key(Key&& key)
{
    swap(*this, key);
}


Key::Key()
{}


Key::~Key()
{
    if (values_)
        delete [] values_;
    if (proofs_)
        delete [] proofs_;
}


Key& Key::operator=(Key key)
{
    swap(*this, key);
    return *this;
}


void swap(Key& a, Key& b)
{
    std::swap(a.values_, b.values_);
    std::swap(a.proofs_, b.proofs_);
    std::swap(a.num_options_, b.num_options_);
}


void Key::serialise(CryptoPP::byte*& output, int& num_options)
{
    num_options = num_options_;
    /*
    Transcript: 4xInteger, 2xbool
    ECPPoint: 1xInteger, 1xbool

    = 5*32 + 3
    = 163 bytes PER OPTION
     */
    size_t length = 163 * num_options_;
    output = new CryptoPP::byte[length];

    for (int i = 0; i < num_options_; i++)
        serialiseSingle((output + 163*i), i);
}


void Key::serialiseSingle(CryptoPP::byte output[163], int option)
{
    CompressedPoint p = CompressPoint(values_[option]);
    p.x.Encode(output, 32);
    output[32] = (p.y == true) ? 1 : 0;

    CryptoPP::ECPPoint* com = proofs_[option].commitment();
    p = CompressPoint(com[0]);
    p.x.Encode(output+33, 32);
    output[65] = (p.y == true) ? 1 : 0;

    p = CompressPoint(com[1]);
    p.x.Encode(output+66, 32);
    output[98] = (p.y == true) ? 1 : 0;

    proofs_[option].challenge().Encode(output+99, 32);
    proofs_[option].response().Encode(output+131, 32);    
}


Key::Key(CryptoPP::byte* input, int num_options,
         const CryptoPP::ECP& ec) : num_options_(num_options)
{
    values_ = new CryptoPP::ECPPoint[num_options_];
    proofs_ = new Transcript[num_options_];

    for (int i = 0; i < num_options_; i++) {
        deserialiseSingle((input + 163*i), i, ec);
    }
}


void Key::deserialiseSingle(CryptoPP::byte input[163], int option, 
                            const CryptoPP::ECP& ec)
{
    CompressedPoint p;
    p.x = CryptoPP::Integer(input, 32);
    p.y = (input[32] == 1) ? true : false;
    values_[option] = DecompressPoint(p, ec);

    CryptoPP::ECPPoint com[2];
    p.x = CryptoPP::Integer(input+33, 32);
    p.y = (input[65] == 1) ? true : false;
    com[0] = DecompressPoint(p, ec);
    p.x = CryptoPP::Integer(input+66, 32);
    p.y = (input[98] == 1) ? true : false;
    com[1] = DecompressPoint(p, ec);
    proofs_[option].setCommitment(com, 2);

    auto e = CryptoPP::Integer(input+99, 32);
    proofs_[option].setChallenge(e);

    auto s = CryptoPP::Integer(input+131, 32);
    proofs_[option].setResponse(s);
}