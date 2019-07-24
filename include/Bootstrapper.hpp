#ifndef BOOTSTRAPPER_HPP
#define BOOTSTRAPPER_HPP


#include "ECGroup.hpp"


class Bootstrapper {
public:
    Bootstrapper(int num_voters, int num_options);
    ~Bootstrapper();

    void readTokensFromFile(const std::string filename);
    void readIDsFromFile(const std::string filename);

private:
    CryptoPP::ECPPoint* voter_ids_;
    int num_voters_;

    CryptoPP::ECPPoint** tokens_;
    int num_options_;
    std::string* options_;

    std::string* ip_addrs_;
};


#endif