#ifndef BOOTSTRAPPER_HPP
#define BOOTSTRAPPER_HPP


#include "ECGroup.hpp"
#include <boost/asio.hpp>


class Bootstrapper {
public:
    Bootstrapper(int num_voters, int num_options);
    ~Bootstrapper();

    void readTokensFromFile(const std::string filename);
    void readIDsFromFile(const std::string filename);
    void readOptionsFromFile(const std::string filename);

    void run();

    inline std::string option(int i) { return options_[i]; }
    inline CryptoPP::ECPPoint voterID(int i) { return voter_ids_[i]; }
    inline CryptoPP::ECPPoint token(int i, int option) { return tokens_[i][option]; }

private:
    int num_voters_;
    int num_options_;

    CryptoPP::ECPPoint* voter_ids_;
    CryptoPP::ECPPoint** tokens_;

    std::string* options_;

    std::string* ip_addrs_;
};


#endif