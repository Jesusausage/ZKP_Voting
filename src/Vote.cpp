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


void Vote::serialise(CryptoPP::byte*& output, int& num_options)
{
    num_options = num_options_;
    /*
    ECPPoint: 1xInteger, 1xbool
    Transcript: 4xInteger, 2xbool
    OrTranscript: 1xInteger, 2xTranscript
    
    Total: 10xInteger, 5xbool

    = 10*33 + 5
    = 335 bytes PER OPTION
    */
    size_t length = 335 * num_options_;
    output = new CryptoPP::byte[length];

    for (int i = 0; i < num_options_; i++)
        serialiseSingle((output + 335*i), i);
}


Vote::Vote(CryptoPP::byte* input, int num_options, 
           const CryptoPP::ECP& ec) : num_options_(num_options)
{
    values_ = new CryptoPP::ECPPoint[num_options_];
    proofs_ = new OrTranscript[num_options_];

    for (int i = 0; i < num_options_; i++) {
        deserialiseSingle((input + 335*i), i, ec);
    }
}


void Vote::serialiseSingle(CryptoPP::byte output[335], int option)
{
    CompressedPoint p = CompressPoint(values_[option]);
    p.x.Encode(output, 33);
    output[33] = (p.y == true) ? 1 : 0;

    Transcript ts = proofs_[option].transcript(0);
    CryptoPP::ECPPoint* com = ts.commitment();
    p = CompressPoint(com[0]);
    p.x.Encode(output+34, 33);
    output[67] = (p.y == true) ? 1 : 0;
    p = CompressPoint(com[1]);
    p.x.Encode(output+68, 33);
    output[101] = (p.y == true) ? 1 : 0;

    ts.challenge().Encode(output+102, 33);
    ts.response().Encode(output+135, 33);

    ts = proofs_[option].transcript(1);
    com = ts.commitment();
    p = CompressPoint(com[0]);
    p.x.Encode(output+168, 33);
    output[201] = (p.y == true) ? 1 : 0;
    p = CompressPoint(com[1]);
    p.x.Encode(output+202, 33);
    output[235] = (p.y == true) ? 1 : 0;

    ts.challenge().Encode(output+236, 33);
    ts.response().Encode(output+269, 33);

    proofs_[option].e().Encode(output+302, 33);
}


void Vote::deserialiseSingle(CryptoPP::byte input[335], int option, 
                             const CryptoPP::ECP& ec)
{
    CompressedPoint p;
    p.x = CryptoPP::Integer(input, 33);
    p.y = (input[33] == 1) ? true : false;
    values_[option] = DecompressPoint(p, ec);

    Transcript ts[2];
    CryptoPP::ECPPoint com[2];
    p.x = CryptoPP::Integer(input+34, 33);
    p.y = (input[67] == 1) ? true : false;
    com[0] = DecompressPoint(p, ec);
    p.x = CryptoPP::Integer(input+68, 33);
    p.y = (input[101] == 1) ? true : false;
    com[1] = DecompressPoint(p, ec);
    ts[0].setCommitment(com, 2);
    ts[0].setChallenge(CryptoPP::Integer(input+102, 33));
    ts[0].setResponse(CryptoPP::Integer(input+135, 33));

    p.x = CryptoPP::Integer(input+168, 33);
    p.y = (input[201] == 1) ? true : false;
    com[0] = DecompressPoint(p, ec);
    p.x = CryptoPP::Integer(input+202, 33);
    p.y = (input[235] == 1) ? true : false;
    com[1] = DecompressPoint(p, ec);
    ts[1].setCommitment(com, 2);
    ts[1].setChallenge(CryptoPP::Integer(input+236, 33));
    ts[1].setResponse(CryptoPP::Integer(input+269, 33));

    auto e = CryptoPP::Integer(input+302, 33);

    proofs_[option] = OrTranscript(ts, 2, e);
}