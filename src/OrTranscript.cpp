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


OrTranscript::OrTranscript()
{}


OrTranscript::~OrTranscript()
{
    if (_transcripts)
        delete [] _transcripts;
}


OrTranscript& OrTranscript::operator=(const OrTranscript& transcript)
{
    if (this == &transcript)
        return *this;
    if (_transcripts)
        delete [] _transcripts;

    _num_prots = transcript._num_prots;
    _e = transcript._e;

    _transcripts = new Transcript[_num_prots];
    for (int i = 0; i < _num_prots; i++) {
        _transcripts[i] = transcript._transcripts[i];
    }

    return *this;
}


Transcript OrTranscript::transcript(int i) const
{
    return _transcripts[i];
}


int OrTranscript::num_prots() const
{
    return _num_prots;
}


CryptoPP::Integer OrTranscript::e() const
{
    return _e;
}