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


void Vote::serialise(CryptoPP::byte* output, int& num_options)
{
    num_options = num_options_;
    /*
    ECPPoint: 1xInteger, 1xbool
    Transcript: 4xInteger, 2xbool
    OrTranscript: 1xInteger(33byte), 2xTranscript
    
    Total: 1xInteger(33byte), 9xInteger, 5xbool

    = 33 + 9*32 + 5
    = 326 bytes PER OPTION
    */

    for (int i = 0; i < num_options_; i++)
        serialiseSingle((output + 326*i), i);
}


Vote::Vote(CryptoPP::byte* input, int num_options, 
           const CryptoPP::ECP& ec) : num_options_(num_options)
{
    values_ = new CryptoPP::ECPPoint[num_options_];
    proofs_ = new OrTranscript[num_options_];

    for (int i = 0; i < num_options_; i++) {
        deserialiseSingle((input + 326*i), i, ec);
    }
}


void Vote::serialiseSingle(CryptoPP::byte output[326], int option)
{
    int offset = 0;
    CompressedPoint p;

    p = CompressPoint(values_[option]);
    p.x.Encode(output+offset, FIELD_SIZE);
    offset += FIELD_SIZE;
    output[offset++] = (p.y == true) ? 1 : 0;

    proofs_[option].serialise(output+offset);
}


void Vote::deserialiseSingle(CryptoPP::byte input[326], int option, 
                             const CryptoPP::ECP& ec)
{
    int offset = 0;
    CompressedPoint p;

    p.x = CryptoPP::Integer(input+offset, FIELD_SIZE);
    offset += FIELD_SIZE;
    p.y = (input[offset++] == 1) ? true : false;
    values_[option] = DecompressPoint(p, ec);

    int r_sizes[2] = {2, 2};
    proofs_[option] = OrTranscript(input+offset, 2, r_sizes, ec);
}


void Vote::hash(char output[32])
{
    std::string hash_data;
    for (int i = 0; i < num_options_; i++) {
        hash_data += CryptoPP::IntToString<CryptoPP::Integer>(values_[i].x);
        hash_data += CryptoPP::IntToString<CryptoPP::Integer>(values_[i].y);
        hash_data += proofs_[i].getHashData();
    }

    CryptoPP::SHA3_256 hash;
    hash.Update((CryptoPP::byte*)hash_data.data(), hash_data.size());
    hash.TruncatedFinal((CryptoPP::byte*)output, 32);
}