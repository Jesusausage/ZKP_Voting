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
    Key();
    ~Key();

    Key& operator=(const Key& key);

    void setValue(int i, CryptoPP::ECPPoint value);
    void setProof(int i, Transcript proof);

    CryptoPP::ECPPoint value(int i) const;
    Transcript proof(int i) const;
    int num_options() const;

private:
    CryptoPP::ECPPoint* _values = nullptr;
    Transcript* _proofs = nullptr;
    int _num_options = 0;
};


#endif