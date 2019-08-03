#ifndef VOTE_DATA_HPP
#define VOTE_DATA_HPP


#include "Verifier.hpp"
#include "Errors.hpp"
#include "TCPServer.hpp"
#include "TCPClient.hpp"
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
    VoteData(const ECGroup& ecg, const CryptoPP::ECPPoint& generator,
             int num_voters, int num_options);
    ~VoteData();

    void processHashes(CryptoPP::byte* hashes, int sender_index);
    void processVKPair(CryptoPP::byte* input, int index);

    boost::asio::const_buffer makeHashesMsg() const;
    boost::asio::const_buffer makeRequestMsg(int index) const;
    boost::asio::const_buffer makeVKPairMsg(int index) const;
       
    static void readVote(int index, CryptoPP::byte* output, int num_options);
    static void readKey(int index, CryptoPP::byte* output, int num_options);
    static void hashTo32(const std::string& hash_data, CryptoPP::byte output[32]);

private:
    const ECGroup& ecg_;
    const CryptoPP::ECPPoint& gen_;
    const int num_voters_;
    const int num_options_;

    std::vector<CryptoPP::ECPPoint> voter_ids_;
    std::vector< std::vector<CryptoPP::ECPPoint> > tokens_;
    std::vector<std::string> options_;
    std::vector<std::string> ip_addrs_;

    std::vector<bool> received_;
    CryptoPP::byte* hashes_;
    std::set< std::array<CryptoPP::byte, 32> > bad_hashes_;

    Verifier* verifier_ = nullptr;

    boost::asio::io_context io_context_;
    TCPClient client_;
    TCPServer server_;    


    void readTokensFromFile();
    void readIDsFromFile();
    void readOptionsFromFile();
    void readIPsFromFile();
    void setVerifier();

    bool validateHash(CryptoPP::byte hash[32], int i);
    void addBadHash(const Vote& vote, const Key& key);
    bool badHash(CryptoPP::byte hash[32]);

    bool verifyVote(const Vote& vote, int index);
    bool verifyKey(const Key& key, int index);

    void writeVote(const Vote& vote, int index);
    void writeKey(const Key& key, int index);
    void writeHash(const Vote& vote, const Key& key, int index);
};




#endif