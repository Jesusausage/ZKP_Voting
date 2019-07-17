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
    Vote(Vote&& vote);
    Vote();
    ~Vote();

    Vote& operator=(Vote vote);
    friend void swap(Vote& a, Vote& b);

    inline void setValue(int i, const CryptoPP::ECPPoint& value) 
        { _values[i] = value; }
    inline void setProof(int i, const OrTranscript& proof) 
        { _proofs[i] = proof; }

    inline CryptoPP::ECPPoint value(int i) const { return _values[i]; }
    inline OrTranscript proof(int i) const { return _proofs[i]; }
    inline int num_options() const { return _num_options; }

private:
    CryptoPP::ECPPoint* _values = nullptr;
    OrTranscript* _proofs = nullptr;
    int _num_options = 0;
};


#endif