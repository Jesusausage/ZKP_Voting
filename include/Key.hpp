#ifndef KEY_HPP
#define KEY_HPP


#include "Transcript.hpp"


class Key {
public:
    Key(CryptoPP::ECPPoint* values,
        Transcript* proofs,
        int num_options);
    Key(int num_options);
    Key(const Key& key);
    Key(Key&& key);
    Key();
    ~Key();

    Key& operator=(Key key);
    friend void swap(Key& a, Key& b);

    inline void setValue(int i, const CryptoPP::ECPPoint& value) 
        { values_[i] = value; }
    inline void setProof(int i, const Transcript& proof) 
        { proofs_[i] = proof; }

    inline CryptoPP::ECPPoint value(int i) const { return values_[i]; }
    inline Transcript proof(int i) const { return proofs_[i]; }
    inline int numOptions() const { return num_options_; }

private:
    CryptoPP::ECPPoint* values_ = nullptr;
    Transcript* proofs_ = nullptr;
    int num_options_ = 0;
};


#endif