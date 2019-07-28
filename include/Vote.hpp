#ifndef VOTE_HPP
#define VOTE_HPP


#include <cryptopp/sha3.h>
#include "ECGroup.hpp"
#include "OrTranscript.hpp"


class Vote {
public:
    Vote(CryptoPP::ECPPoint* values,
         OrTranscript* proofs,
         int num_options);
    Vote(int num_options);
    Vote(const Vote& vote);
    Vote(Vote&& vote);
    Vote();
    ~Vote();

    Vote& operator=(Vote vote);
    friend void swap(Vote& a, Vote& b);

    inline void setValue(int i, const CryptoPP::ECPPoint& value) 
        { values_[i] = value; }
    inline void setProof(int i, const OrTranscript& proof) 
        { proofs_[i] = proof; }

    inline CryptoPP::ECPPoint value(int i) const { return values_[i]; }
    inline OrTranscript proof(int i) const { return proofs_[i]; }
    inline int numOptions() const { return num_options_; }

    void serialise(CryptoPP::byte* output, int& num_options) const;
    Vote(CryptoPP::byte* input, int num_options, const CryptoPP::ECP& ec);

    void hash(char output[32]) const;

private:
    CryptoPP::ECPPoint* values_ = nullptr;
    OrTranscript* proofs_ = nullptr;
    int num_options_ = 0;

    void serialiseSingle(CryptoPP::byte output[326], int option) const;
    void deserialiseSingle(CryptoPP::byte input[326], int option, 
                           const CryptoPP::ECP& ec);
};


#endif