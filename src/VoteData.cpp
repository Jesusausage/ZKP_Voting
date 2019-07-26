#include "VoteData.hpp"


VoteData::VoteData(int num_voters, int num_options)
                           :
                           num_voters_(num_voters), 
                           num_options_(num_options)
{
    voter_ids_ = new CryptoPP::ECPPoint[num_voters_];

    tokens_ = new CryptoPP::ECPPoint*[num_voters_];
    for (int i = 0; i < num_voters_; i++)
        tokens_[i] = new CryptoPP::ECPPoint[num_options_];

    options_ = new std::string[num_options_];

    ip_addrs_ = new std::string[num_voters_];
}


VoteData::~VoteData()
{
    delete [] voter_ids_;
    for (int i = 0; i < num_voters_; i++)
        delete [] tokens_[i];
    delete [] tokens_;

    delete [] options_;
    delete [] ip_addrs_;
}


void VoteData::readTokensFromFile(const std::string filename)
{
    std::ifstream token_in(filename);
    ReadTokens(token_in, tokens_, num_options_);
    token_in.close();
}


void VoteData::readIDsFromFile(const std::string filename)
{
    std::ifstream id_in(filename);
    ReadIDs(id_in, voter_ids_);
    id_in.close();
}


void VoteData::readOptionsFromFile(const std::string filename)
{
    std::ifstream options_in(filename);
    int i = 0;

    do
        options_in >> options_[i++];
    while (!options_in.eof());
}