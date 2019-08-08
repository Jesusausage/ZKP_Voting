#ifndef PRIV_DATA_HPP
#define PRIV_DATA_HPP


#include "PublicData.hpp"
#include "Errors.hpp"
#include <vector>
#include <fstream>


#define VOTE_FILE "output/votes"
#define KEY_FILE "output/keys"
#define PRIV_KEY_FILE "private_keys.txt"


class PrivateData
{
public:
    PrivateData(const ECGroup &ecg, const CryptoPP::ECPPoint &generator,
                const PublicData &pub_data);

    inline int voterIndex() const 
        { return voter_index_; }
    inline const CryptoPP::Integer& idKey() const
        { return id_key_; }
    inline const std::vector<CryptoPP::Integer>& tokenKeys() const
        { return token_keys_; }

private:
    const PublicData &dat_;

    int voter_index_;
    CryptoPP::Integer id_key_;
    std::vector<CryptoPP::Integer> token_keys_;

    void readPrivFromFile();
    void findVoterIndex(const ECGroup& ecg, const CryptoPP::ECPPoint& gen);
};


#endif