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
        { _values[i] = value; }
    inline void setProof(int i, const Transcript& proof) 
        { _proofs[i] = proof; }

    inline CryptoPP::ECPPoint value(int i) const { return _values[i]; }
    inline Transcript proof(int i) const { return _proofs[i]; }
    inline int num_options() const { return _num_options; }

private:
    CryptoPP::ECPPoint* _values = nullptr;
    Transcript* _proofs = nullptr;
    int _num_options = 0;
};


#endif