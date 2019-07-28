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


void VoteData::readTokensFromFile(const std::string& filename /*= TOKEN_FILE*/)
{
    std::ifstream token_in(filename);
    ReadTokens(token_in, tokens_, num_options_);
    token_in.close();
}


void VoteData::readIDsFromFile(const std::string& filename /*= ID_FILE*/)
{
    std::ifstream id_in(filename);
    ReadIDs(id_in, voter_ids_);
    id_in.close();
}


void VoteData::readOptionsFromFile(const std::string& filename /*= OPTION_FILE*/)
{
    std::ifstream options_in(filename);
    int i = 0;

    do
        options_in >> options_[i++];
    while (!options_in.eof());
    options_in.close();
}


void VoteData::readIPsFromFile(const std::string& filename /*= IP_FILE*/)
{
    std::ifstream ips_in(filename);
    int i = 0;

    do
        ips_in >> ip_addrs_[i++];
    while (!ips_in.eof());    
    ips_in.close();
}


void VoteData::processHashes(char** key_hashes, char** vote_hashes,
                             const std::string& ip)
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


bool VoteData::validateHash(char key_hash[32], char vote_hash[32], int i)
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
    auto* token_sums = new CryptoPP::ECPPoint[num_options_];
    for (int i = 0; i < num_voters_; i++) {
        id_sum = ecg.curve.Add(id_sum, voter_ids_[i]);
        for (int option = 0; option < num_options_; option++) {
            token_sums[option] = ecg.curve.Add(token_sums[option],
                                               tokens_[i][option]);
        }
    }

    verifier_ = new Verifier(ecg, generator, id_sum, token_sums, num_options_);
    delete [] token_sums;
}


Vote VoteData::requestVote(const std::string& ip, int index)
{
    Vote vote; // ask "ip" for vote[index]
    return vote;
}


Key VoteData::requestKey(const std::string& ip, int index)
{
    Key key; // ask "ip" for key[index]
    return key;
}


bool VoteData::verifyVote(const Vote& vote, int index)
{
    verifier_->setTokens(tokens_[index]); 
    return verifier_->verifyVote(vote);
}


bool VoteData::verifyKey(const Key& key, int index)
{
    verifier_->setID(voter_ids_[index]);
    return verifier_->verifyKey(key);
}


void VoteData::writeVote(const Vote& vote, int index)
{
    std::string filename = VOTE_FILE;
    filename += std::to_string(index);
    filename += ".txt";

    vote.hash(vote_hashes_[index]);

    std::ofstream vote_out(filename);
    for (int i = 0; i < num_options_; i++) {
        vote_out << vote.value(i).x << " " << vote.value(i).y << std::endl;
    }
    vote_out.close();
}


void VoteData::writeKey(const Key& key, int index)
{
    std::string filename = KEY_FILE;
    filename += std::to_string(index);
    filename += ".txt";

    key.hash(key_hashes_[index]);

    std::ofstream key_out(filename);
    for (int i = 0; i < num_options_; i++) {
        key_out << key.value(i).x << " " << key.value(i).y << std::endl;
    }
    key_out.close();
}