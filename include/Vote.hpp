#ifndef VOTE_HPP
#define VOTE_HPP


#include "OrTranscript.hpp"


class Vote {
public:
    Vote(CryptoPP::ECPPoint* values,
         OrTranscript* proofs,
         int num_options);
    Vote(int num_options);
    Vote(const Vote& vote);
    Vote();
    ~Vote();

    Vote& operator=(const Vote& vote);

    void setValue(int i, CryptoPP::ECPPoint value);
    void setProof(int i, OrTranscript proof);

    CryptoPP::ECPPoint value(int i) const;
    OrTranscript proof(int i) const;
    int num_options() const;

private:
    CryptoPP::ECPPoint* _values = nullptr;
    OrTranscript* _proofs = nullptr;
    int _num_options = 0;
};


#endif