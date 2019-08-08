#ifndef PUB_DATA_HPP
#define PUB_DATA_HPP


#include "ECGroup.hpp"
#include "Errors.hpp"
#include <vector>
#include <fstream>


#define TOKEN_FILE "tokens.txt"
#define ID_FILE "IDs.txt"
#define OPTION_FILE "options.txt"
#define IP_FILE "IPs.txt"


class PublicData {
public:
    PublicData(int num_voters, int num_options);

    inline int numVoters() const
        { return num_voters_; }
    inline int numOptions() const
        { return num_options_; }

    inline const CryptoPP::ECPPoint& id(int index) const
        { return voter_ids_[index]; }
    inline const std::vector<CryptoPP::ECPPoint> tokens(int index) const 
        { return tokens_[index]; }
    inline const std::string option(int index) const 
        { return options_[index]; }
    inline const std::string ip(int index) const 
        { return ip_addrs_[index]; }

private:
    const int num_voters_;
    const int num_options_;
    std::vector<CryptoPP::ECPPoint> voter_ids_;
    std::vector< std::vector<CryptoPP::ECPPoint> > tokens_;
    std::vector<std::string> options_;
    std::vector<std::string> ip_addrs_;


    void readTokensFromFile();
    void readIDsFromFile();
    void readOptionsFromFile();
    void readIPsFromFile();
};


#endif