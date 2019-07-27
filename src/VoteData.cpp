#include "VoteData.hpp"


VoteData::VoteData(int num_voters, int num_options)
                           :
                           num_voters_(num_voters), 
                           num_options_(num_options)
{
    voter_ids_ = new CryptoPP::ECPPoint[num_voters_];
    tokens_ = new CryptoPP::ECPPoint*[num_voters_];
    key_hashes_ = new char*[num_voters_];
    vote_hashes_ = new char*[num_voters_];

    for (int i = 0; i < num_voters_; i++) {
        tokens_[i] = new CryptoPP::ECPPoint[num_options_];
        key_hashes_[i] = new char[32];
        vote_hashes_[i] = new char[32];
    }

    options_ = new std::string[num_options_];
    ip_addrs_ = new std::string[num_voters_];
}


VoteData::~VoteData()
{
    for (int i = 0; i < num_voters_; i++) {
        delete [] tokens_[i];
        delete [] key_hashes_[i];
        delete [] vote_hashes_[i];
    }

    delete [] voter_ids_;
    delete [] tokens_;
    delete [] key_hashes_;
    delete [] vote_hashes_;

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


void VoteData::readIPsFromFile(const std::string filename)
{
    std::ifstream ips_in(filename);
    int i = 0;

    do
        ips_in >> ip_addrs_[i++];
    while (!ips_in.eof());    
}