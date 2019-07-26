#ifndef VOTE_DATA_HPP
#define VOTE_DATA_HPP


#include "ECGroup.hpp"


class VoteData {
public:
    VoteData(int num_voters, int num_options);
    ~VoteData();

    void readTokensFromFile(const std::string filename);
    void readIDsFromFile(const std::string filename);
    void readOptionsFromFile(const std::string filename);

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