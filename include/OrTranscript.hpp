#ifndef OR_TRANSCRIPT_HPP
#define OR_TRANSCRIPT_HPP


#include "Transcript.hpp"


class OrTranscript {
public:
    OrTranscript(Transcript* transcripts, 
                 int num_prots, 
                 const CryptoPP::Integer& e);
    OrTranscript(const OrTranscript& or_transcript);
    OrTranscript(OrTranscript&& or_transcript);
    OrTranscript();
    ~OrTranscript();

    OrTranscript& operator=(OrTranscript or_transcript);
    friend void swap(OrTranscript& a, OrTranscript& b);

    inline Transcript transcript(int i) const { return transcripts_[i]; }
    inline int numProts() const { return num_prots_; }
    inline CryptoPP::Integer e() const { return e_; }

private:
    Transcript* transcripts_ = nullptr;
    int num_prots_ = 0;
    CryptoPP::Integer e_ = 0;
};


#endif