#ifndef BOOTSTRAPPER_HPP
#define BOOTSTRAPPER_HPP


#include "ECGroup.hpp"


class Bootstrapper {
public:

private:
    CryptoPP::ECPPoint* voter_ids_;
    int num_voters;

    CryptoPP::ECPPoint** tokens_;
    int num_options_;
    std::string* options_;

    std::string* ip_addrs_;

};


#endif