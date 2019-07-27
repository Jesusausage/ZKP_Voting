#include "VoteData.hpp"


VoteData::VoteData(int num_voters, int num_options)
                   :
                   num_voters_(num_voters), 
                   num_options_(num_options)
{
    voter_ids_ = new CryptoPP::ECPPoint[num_voters_];
    tokens_ = new CryptoPP::ECPPoint*[num_voters_];
    key_hashes_ = new char*[num_voters_];
    vote_hashes_ = new char*[num_voters_];

    for (int i = 0; i < num_voters_; i++) {
        tokens_[i] = new CryptoPP::ECPPoint[num_options_];
        key_hashes_[i] = new char[32];
        vote_hashes_[i] = new char[32];
    }

    options_ = new std::string[num_options_];
    ip_addrs_ = new std::string[num_voters_];
}


VoteData::~VoteData()
{
    for (int i = 0; i < num_voters_; i++) {
        delete [] tokens_[i];
        delete [] key_hashes_[i];
        delete [] vote_hashes_[i];
    }

    delete [] voter_ids_;
    delete [] tokens_;
    delete [] key_hashes_;
    delete [] vote_hashes_;

    delete [] options_;
    delete [] ip_addrs_;

    if (verifier_)
        delete verifier_;
}


void VoteData::readTokensFromFile(const std::string filename)
{
    std::ifstream token_in(filename);
    ReadTokens(token_in, tokens_, num_options_);
    token_in.close();
}


void VoteData::readIDsFromFile(const std::string filename)
{
    std::ifstream id_in(filename);
    ReadIDs(id_in, voter_ids_);
    id_in.close();
}


void VoteData::readOptionsFromFile(const std::string filename)
{
    std::ifstream options_in(filename);
    int i = 0;

    do
        options_in >> options_[i++];
    while (!options_in.eof());
}


void VoteData::readIPsFromFile(const std::string filename)
{
    std::ifstream ips_in(filename);
    int i = 0;

    do
        ips_in >> ip_addrs_[i++];
    while (!ips_in.eof());    
}


bool VoteData::processHashes(char** key_hashes, char** vote_hashes,
                             const std::string ip)
{
    for (int i = 0; i < num_voters_; i++) {
        if (!validateHash(key_hashes[i], vote_hashes[i], i, ip)) {
            auto vote = requestVote(ip, i);
            if (verifyVote(vote, i))
                writeVote(vote, i);

            auto key = requestKey(ip, i);
            if (verifyKey(key, i))
                writeKey(key, i);
        }
    }
}


bool VoteData::validateHash(char key_hash[32], char vote_hash[32],
                            int i, const std::string ip)
{
    for (int ch = 0; ch < 32; ch++) {
        if (key_hash[ch] != key_hashes_[i][ch] ||
            vote_hash[ch] != vote_hashes_[i][ch]) {
            return false;
        }
    }

    return true;
}


void VoteData::setVerifier(const ECGroup& ecg, 
                           const CryptoPP::ECPPoint& generator)
{
    CryptoPP::ECPPoint id_sum;
    CryptoPP::ECPPoint token_sums[num_options_];
    for (int i = 0; i < num_voters_; i++) {
        id_sum = ecg.curve.Add(id_sum, voter_ids_[i]);
        for (int option = 0; option < num_options_; option++) {
            token_sums[option] = ecg.curve.Add(token_sums[option],
                                               tokens_[i][option]);
        }
    }

    verifier_ = new Verifier(ecg, generator, id_sum, token_sums, num_options_);
}


Vote VoteData::requestVote(const std::string ip, int index)
{
    Vote vote; // ask "ip" for vote[index]
    return vote;
}


Key VoteData::requestKey(const std::string ip, int index)
{
    Key key; // ask "ip" for key[index]
    return key;
}


bool VoteData::verifyVote(const Vote& vote, int index)
{
    verifier_->setTokens(tokens_[index]); 
    verifier_->verifyVote(vote);
}


bool VoteData::verifyKey(const Key& key, int index)
{
    verifier_->setID(voter_ids_[index]);
    verifier_->verifyKey(key);
}


void VoteData::writeVote(const Vote& vote, int index)
{
    // write new hash
    // write new vote
}


void VoteData::writeKey(const Key& key, int index)
{
    // write new hash
    // write new key
}