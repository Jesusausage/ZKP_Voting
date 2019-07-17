#include "OrTranscript.hpp"


OrTranscript::OrTranscript(Transcript* transcripts, 
                           int num_prots, 
                           const CryptoPP::Integer& e)
                           :
                           _num_prots(num_prots),
                           _e(e)
{
    _transcripts = new Transcript[_num_prots];
    for (int i = 0; i < _num_prots; i++) {
        _transcripts[i] = transcripts[i];
    }
}


OrTranscript::OrTranscript(const OrTranscript& or_transcript)
                           :
                           _num_prots(or_transcript._num_prots),
                           _e(or_transcript._e)
{
    _transcripts = new Transcript[_num_prots];
    for (int i = 0; i < _num_prots; i++) {
        _transcripts[i] = or_transcript._transcripts[i];
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
    if (_transcripts)
        delete [] _transcripts;
}


OrTranscript& OrTranscript::operator=(OrTranscript or_transcript)
{
    swap(*this, or_transcript);
    return *this;
}


void swap(OrTranscript& a, OrTranscript& b)
{
    std::swap(a._transcripts, b._transcripts);
    std::swap(a._num_prots, b._num_prots);
    std::swap(a._e, b._e);
}