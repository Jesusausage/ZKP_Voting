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


void OrTranscript::serialise(CryptoPP::byte* output, 
                             int* num_prots /*= nullptr*/,
                             int* commitment_sizes /*= nullptr*/)
{
    if (num_prots)
        *num_prots = num_prots_;

    int offset = 0;
    int r_size;

    for (int i = 0; i < num_prots_; i++) {
        transcripts_[i].serialise(output+offset, &r_size);
        offset += 2*FIELD_SIZE + r_size*(FIELD_SIZE + 1);
        if (commitment_sizes)
            commitment_sizes[i] = r_size;
    }

    e_.Encode(output+offset, FIELD_SIZE+1);
}


OrTranscript::OrTranscript(CryptoPP::byte* input, 
                           int num_prots,
                           int* commitment_sizes,
                           const CryptoPP::ECP& ec) 
                           : 
                           num_prots_(num_prots)
{
    transcripts_ = new Transcript[num_prots_];
    int offset = 0;

    for (int i = 0; i < num_prots_; i++) {
        transcripts_[i] = Transcript(input+offset, commitment_sizes[i], ec);
        offset +=  2*FIELD_SIZE + commitment_sizes[i]*(FIELD_SIZE + 1);
    }

    e_ = CryptoPP::Integer(input+offset, FIELD_SIZE+1);
}


std::string OrTranscript::getHashData()
{
    std::string ret;

    for (int i = 0; i < num_prots_; i++)
        ret += transcripts_[i].getHashData();

    return ret;
}