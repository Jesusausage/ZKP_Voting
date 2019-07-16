#ifndef OR_TRANSCRIPT_HPP
#define OR_TRANSCRIPT_HPP


#include "Transcript.hpp"


class OrTranscript {
public:
    OrTranscript(Transcript* transcripts, 
                 int num_prots, 
                 const CryptoPP::Integer& e);
    OrTranscript(const OrTranscript& or_transcript);
    OrTranscript();
    ~OrTranscript();

    OrTranscript& operator=(const OrTranscript& transcript);

    Transcript transcript(int i) const;
    int num_prots() const;
    CryptoPP::Integer e() const;

private:
    Transcript* _transcripts = nullptr;
    int _num_prots = 0;
    CryptoPP::Integer _e = 0;
};


#endif