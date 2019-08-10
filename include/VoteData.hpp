#ifndef VOTE_DATA_HPP
#define VOTE_DATA_HPP


#include "PublicData.hpp"
#include "PrivateData.hpp"
#include "Verifier.hpp"
#include <array>
#include <set>


#define VOTE_FILE "output/votes"
#define KEY_FILE "output/keys"


class VoteData {
    friend class VoteDataTest;
public:
    VoteData(const ECGroup& ecg, const CryptoPP::ECPPoint& generator,
             const PublicData& pub_data, const PrivateData& priv_data);
    ~VoteData();

    int processReceived(bool received[]);
    void processVKPair(CryptoPP::byte* input, int index);

    inline bool* received() const
        { return received_; }

    inline int numOptions() const
        { return pub_.numOptions(); }
    inline int numVoters() const
        { return pub_.numVoters(); }

    std::string randomIP() const;
       
    static void readVote(int index, CryptoPP::byte* output, int num_options);
    static void readKey(int index, CryptoPP::byte* output, int num_options);

private:
    const ECGroup& ecg_;
    const CryptoPP::ECPPoint& gen_;

    const PublicData& pub_;
    const PrivateData& priv_;

    int voter_index_;
    Verifier* verifier_ = nullptr;
    bool* received_;

    bool verifyVote(const Vote& vote, int index);
    bool verifyKey(const Key& key, int index);

    void writeVote(const Vote& vote, int index);
    void writeKey(const Key& key, int index);

    void findVoterIndex();

    void getUserVote();
    int getUserInput();
};




#endif