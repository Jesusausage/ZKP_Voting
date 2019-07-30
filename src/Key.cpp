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


void Key::serialise(CryptoPP::byte* output, int& num_options) const
{
    num_options = num_options_;
    /*
    Transcript: 4xInteger, 2xbool
    ECPPoint: 1xInteger, 1xbool

    = 5*32 + 3
    = 163 bytes PER OPTION
     */

    for (int i = 0; i < num_options_; i++)
        serialiseSingle((output + 163*i), i);
}


void Key::serialiseSingle(CryptoPP::byte output[163], int option) const
{
    int offset = 0;
    CompressedPoint p;

    p = CompressPoint(values_[option]);
    p.x.Encode(output+offset, FIELD_SIZE);
    offset += FIELD_SIZE;
    output[offset++] = (p.y == true) ? 1 : 0;

    proofs_[option].serialise(output+offset);
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
    int offset = 0;
    CompressedPoint p;

    p.x = CryptoPP::Integer(input+offset, FIELD_SIZE);
    offset += FIELD_SIZE;
    p.y = (input[offset++] == 1) ? true : false;
    values_[option] = DecompressPoint(p, ec);

    proofs_[option] = Transcript(input+offset, 2, ec);
}


std::string Key::getHashData() const
{    
    std::string hash_data;
    for (int i = 0; i < num_options_; i++) {
        hash_data += CryptoPP::IntToString<CryptoPP::Integer>(values_[i].x);
        hash_data += CryptoPP::IntToString<CryptoPP::Integer>(values_[i].y);
        hash_data += proofs_[i].getHashData();
    }
    return hash_data;
}