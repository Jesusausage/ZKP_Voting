#ifndef VOTE_DATA_HPP
#define VOTE_DATA_HPP


#include "Verifier.hpp"

#define VOTE_FILE "votes.txt"
#define KEY_FILE "keys.txt"


class VoteData {
public:
    VoteData(int num_voters, int num_options);
    ~VoteData();

    void readTokensFromFile(const std::string filename);
    void readIDsFromFile(const std::string filename);
    void readOptionsFromFile(const std::string filename);
    void readIPsFromFile(const std::string filename);

    inline std::string option(int i) const 
        { return options_[i]; }
    inline CryptoPP::ECPPoint voterID(int i) const 
        { return voter_ids_[i]; }
    inline CryptoPP::ECPPoint token(int i, int option) const
        { return tokens_[i][option]; }
    inline std::string ip(int i) const 
        { return ip_addrs_[i]; }

    void validateHashes(char** key_hashes, char** vote_hashes,
                        const std::string ip);

private:
    const int num_voters_;
    const int num_options_;

    CryptoPP::ECPPoint* voter_ids_;
    CryptoPP::ECPPoint** tokens_;
    Verifier* verifier;

    char** key_hashes_;
    char** vote_hashes_;

    std::string* options_;

    std::string* ip_addrs_;

    void requestVote(const std::string ip, int i);
    void requestKey(const std::string ip, int i);
};


#endif