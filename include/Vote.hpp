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
        { values_[i] = value; }
    inline void setProof(int i, const OrTranscript& proof) 
        { proofs_[i] = proof; }

    inline CryptoPP::ECPPoint value(int i) const { return values_[i]; }
    inline OrTranscript proof(int i) const { return proofs_[i]; }
    inline int numOptions() const { return num_options_; }

private:
    CryptoPP::ECPPoint* values_ = nullptr;
    OrTranscript* proofs_ = nullptr;
    int num_options_ = 0;
};


#endif