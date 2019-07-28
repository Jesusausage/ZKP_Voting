#ifndef VOTE_DATA_HPP
#define VOTE_DATA_HPP


#include "Verifier.hpp"

#define VOTE_FILE "output/votes"
#define KEY_FILE "output/keys"


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

    void processHashes(char** key_hashes, char** vote_hashes,
                       const std::string ip);

    void setVerifier(const ECGroup& ecg,
                     const CryptoPP::ECPPoint& generator);

    void writeVote(const Vote& vote, int index);
    void writeKey(const Key& key, int index);

private:
    const int num_voters_;
    const int num_options_;

    CryptoPP::ECPPoint* voter_ids_;
    CryptoPP::ECPPoint** tokens_;

    char** key_hashes_;
    char** vote_hashes_;

    std::string* options_;
    std::string* ip_addrs_;

    Verifier* verifier_ = nullptr;


    bool validateHash(char key_hashes[32], char vote_hashes[32],
                      int i, const std::string ip);

    Vote requestVote(const std::string ip, int index);
    Key requestKey(const std::string ip, int index);

    bool verifyVote(const Vote& vote, int index);
    bool verifyKey(const Key& key, int index);
};


#endif