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