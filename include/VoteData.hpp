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
    friend class VoteDataTest;
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

    Vote readVote(int index);
    Key readKey(int index);

    void processHashes(CryptoPP::byte** hashes, int sender_index);

    void setVerifier(const ECGroup& ecg,
                     const CryptoPP::ECPPoint& generator);

private:
    const int num_voters_;
    const int num_options_;

    std::vector<CryptoPP::ECPPoint> voter_ids_;
    std::vector< std::vector<CryptoPP::ECPPoint> > tokens_;

    CryptoPP::byte** hashes_;
    std::set< std::array<CryptoPP::byte, 32> > bad_hashes_;

    std::vector<std::string> options_;
    std::vector<std::string> ip_addrs_;

    Verifier* verifier_ = nullptr;


    bool validateHash(CryptoPP::byte hash[32], int i);
    void addBadHash(CryptoPP::byte bad_hash[32]);
    bool badHash(CryptoPP::byte hash[32]);

    Vote requestVote(int sender_index, int vote_index);
    Key requestKey(int sender_index, int key_index);

    bool verifyVote(const Vote& vote, int index);
    bool verifyKey(const Key& key, int index);

    void writeVote(const Vote& vote, int index);
    void writeKey(const Key& key, int index);
    void writeHash(const Vote& vote, const Key& key, int index);
};


void HashTo32(const std::string& hash_data, CryptoPP::byte output[32]);


#endif