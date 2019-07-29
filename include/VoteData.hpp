#ifndef VOTE_DATA_HPP
#define VOTE_DATA_HPP


#include "Verifier.hpp"
#include <array>
#include <set>


#define VOTE_FILE "output/votes"
#define KEY_FILE "output/keys"

#define TOKEN_FILE "tokens.txt"
#define ID_FILE "IDs.txt"
#define OPTION_FILE "options.txt"
#define IP_FILE "IPs.txt"


class VoteData {
public:
    VoteData(int num_voters, int num_options);
    ~VoteData();

    inline std::string option(int i) const 
        { return options_[i]; }
    inline CryptoPP::ECPPoint voterID(int i) const 
        { return voter_ids_[i]; }
    inline CryptoPP::ECPPoint token(int i, int option) const
        { return tokens_[i][option]; }
    inline std::string ip(int i) const 
        { return ip_addrs_[i]; }

    void readTokensFromFile(const std::string& filename = TOKEN_FILE);
    void readIDsFromFile(const std::string& filename = ID_FILE);
    void readOptionsFromFile(const std::string& filename = OPTION_FILE);
    void readIPsFromFile(const std::string& filename = IP_FILE);

    void processHashes(char** key_hashes, char** vote_hashes,
                       int sender_index);

    void setVerifier(const ECGroup& ecg,
                     const CryptoPP::ECPPoint& generator);

private:
    const int num_voters_;
    const int num_options_;

    std::vector<CryptoPP::ECPPoint> voter_ids_;
    std::vector< std::vector<CryptoPP::ECPPoint> > tokens_;

    std::vector< std::array<char, 32> > key_hashes_;
    std::vector< std::array<char, 32> > vote_hashes_;

    std::vector<std::string> options_;
    std::vector<std::string> ip_addrs_;
    std::set<int> bad_senders_;

    Verifier* verifier_ = nullptr;


    bool validateHash(char key_hashes[32], char vote_hashes[32], int i);

    Vote requestVote(int sender_index, int vote_index);
    Key requestKey(int sender_index, int key_index);

    bool verifyVote(const Vote& vote, int index);
    bool verifyKey(const Key& key, int index);

    void writeVote(const Vote& vote, int index);
    void writeKey(const Key& key, int index);
};


#endif