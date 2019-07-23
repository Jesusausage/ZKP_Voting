#include "Bootstrapper.hpp"


Bootstrapper::Bootstrapper(int num_voters, int num_options)
                           :
                           num_voters_(num_voters), num_options_(num_options)
{
    voter_ids_ = new CryptoPP::ECPPoint[num_voters_];

    tokens_ = new CryptoPP::ECPPoint*[num_voters_];
    for (int i = 0; i < num_voters_; i++)
        tokens_[i] = new CryptoPP::ECPPoint[num_options_];

    options_ = new std::string[num_options_];

    ip_addrs_ = new std::string[num_voters_];
}


Bootstrapper::~Bootstrapper()
{
    delete [] voter_ids_;
    for (int i = 0; i < num_voters_; i++)
        delete [] tokens_[i];
    delete [] tokens_;

    delete [] options_;
    delete [] ip_addrs_;
}