#ifndef VOTE_DATA_HPP
#define VOTE_DATA_HPP


#include "Verifier.hpp"
#include "Errors.hpp"
#include <array>
#include <set>


#define VOTE_FILE "output/votes"
#define KEY_FILE "output/keys"

#define TOKEN_FILE "tokens.txt"
#define ID_FILE "IDs.txt"
#define OPTION_FILE "options.txt"
#define IP_FILE "IPs.txt"

#define PRIV_KEY_FILE "private_keys.txt"


class VoteData {
    friend class VoteDataTest;
public:
    VoteData(const ECGroup& ecg, const CryptoPP::ECPPoint& generator,
             int num_voters, int num_options);
    ~VoteData();

    int processReceived(bool received[]);
    void processVKPair(CryptoPP::byte* input, int index);

    inline bool* received() const
        { return received_; }

    inline int numOptions() const
        { return num_options_; }
    inline int numVoters() const
        { return num_voters_; }

    std::string randomIP() const;
       
    static void readVote(int index, CryptoPP::byte* output, int num_options);
    static void readKey(int index, CryptoPP::byte* output, int num_options);

private:
    const ECGroup& ecg_;
    const CryptoPP::ECPPoint& gen_;
    const int num_voters_;
    const int num_options_;

    std::vector<CryptoPP::ECPPoint> voter_ids_;
    std::vector< std::vector<CryptoPP::ECPPoint> > tokens_;
    CryptoPP::ECPPoint id_sum_;
    std::vector<CryptoPP::ECPPoint> token_sums_;
    std::vector<std::string> options_;
    std::vector<std::string> ip_addrs_;

    bool* received_;

    Verifier* verifier_ = nullptr;

    int voter_index_;
    CryptoPP::Integer id_key_;
    std::vector<CryptoPP::Integer> token_keys_;


    void readTokensFromFile();
    void readIDsFromFile();
    void readOptionsFromFile();
    void readIPsFromFile();
    void setSums();

    void getUserVote();
    void readPrivFromFile();

    bool verifyVote(const Vote& vote, int index);
    bool verifyKey(const Key& key, int index);

    void writeVote(const Vote& vote, int index);
    void writeKey(const Key& key, int index);
};




#endif