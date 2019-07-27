#ifndef KEY_HPP
#define KEY_HPP


#include <cryptopp/sha3.h>
#include "ECGroup.hpp"
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

    void serialise(CryptoPP::byte* output, int& num_options);
    Key(CryptoPP::byte* input, int num_options, const CryptoPP::ECP& ec);

    void hash(char output[32]);

private:
    CryptoPP::ECPPoint* values_ = nullptr;
    Transcript* proofs_ = nullptr;
    int num_options_ = 0;

    void serialiseSingle(CryptoPP::byte output[163], int option);
    void deserialiseSingle(CryptoPP::byte input[163], int option, 
                           const CryptoPP::ECP& ec);
};


#endif