#include "OrTranscript.hpp"


OrTranscript::OrTranscript(Transcript* transcripts, 
                           int num_prots, 
                           const CryptoPP::Integer& e)
                           :
                           num_prots_(num_prots),
                           e_(e)
{
    transcripts_ = new Transcript[num_prots_];
    for (int i = 0; i < num_prots_; i++) {
        transcripts_[i] = transcripts[i];
    }
}


OrTranscript::OrTranscript(const OrTranscript& or_transcript)
                           :
                           num_prots_(or_transcript.num_prots_),
                           e_(or_transcript.e_)
{
    transcripts_ = new Transcript[num_prots_];
    for (int i = 0; i < num_prots_; i++) {
        transcripts_[i] = or_transcript.transcripts_[i];
    }
}


OrTranscript::OrTranscript(OrTranscript&& or_transcript)
{
    swap(*this, or_transcript);
}


OrTranscript::OrTranscript()
{}


OrTranscript::~OrTranscript()
{
    if (transcripts_)
        delete [] transcripts_;
}


OrTranscript& OrTranscript::operator=(OrTranscript or_transcript)
{
    swap(*this, or_transcript);
    return *this;
}


void swap(OrTranscript& a, OrTranscript& b)
{
    std::swap(a.transcripts_, b.transcripts_);
    std::swap(a.num_prots_, b.num_prots_);
    std::swap(a.e_, b.e_);
}