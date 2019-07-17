#include "Vote.hpp"


Vote::Vote(CryptoPP::ECPPoint* values,
           OrTranscript* proofs,
           int num_options)
           :
           num_options_(num_options)
{
    values_ = new CryptoPP::ECPPoint[num_options_];
    proofs_ = new OrTranscript[num_options_];
    for (int i = 0; i < num_options_; i++) {
        values_[i] = values[i];
        proofs_[i] = proofs[i];
    }
}


Vote::Vote(int num_options) : num_options_(num_options)
{
    values_ = new CryptoPP::ECPPoint[num_options_];
    proofs_ = new OrTranscript[num_options_];
}


Vote::Vote(const Vote& vote) : num_options_(vote.num_options_)
{
    values_ = new CryptoPP::ECPPoint[num_options_];
    proofs_ = new OrTranscript[num_options_];
    for (int i = 0; i < num_options_; i++) {
        values_[i] = vote.values_[i];
        proofs_[i] = vote.proofs_[i];
    }
}


Vote::Vote(Vote&& vote)
{
    swap(*this, vote);
}


Vote::Vote()
{}


Vote::~Vote()
{
    if (values_)
        delete [] values_;
    if (proofs_)
        delete [] proofs_;
}


Vote& Vote::operator=(Vote vote)
{
    swap(*this, vote);
    return *this;
}


void swap(Vote& a, Vote& b)
{
    std::swap(a.values_, b.values_);
    std::swap(a.proofs_, b.proofs_);
    std::swap(a.num_options_, b.num_options_);
}