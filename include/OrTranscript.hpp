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

    inline Transcript transcript(int i) const { return _transcripts[i]; }
    inline int num_prots() const { return _num_prots; }
    inline CryptoPP::Integer e() const { return _e; }

private:
    Transcript* _transcripts = nullptr;
    int _num_prots = 0;
    CryptoPP::Integer _e = 0;
};


#endif