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

    const inline Transcript& transcript(int i) const 
        { return transcripts_[i]; }
    const inline int& numProts() const 
        { return num_prots_; }
    const inline CryptoPP::Integer& e() const 
        { return e_; }

    void serialise(CryptoPP::byte* output, 
                   int* num_prots = nullptr,
                   int* commitment_sizes = nullptr);
    OrTranscript(CryptoPP::byte* input, 
                 int num_prots, 
                 int* commitment_sizes, 
                 const CryptoPP::ECP& ec);

    std::string getHashData();

private:
    Transcript* transcripts_ = nullptr;
    int num_prots_ = 0;
    CryptoPP::Integer e_ = 0;
};


#endif