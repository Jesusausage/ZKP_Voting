#include "VoteData.hpp"


VoteData::VoteData(int num_voters, int num_options)
                   :
                   num_voters_(num_voters), 
                   num_options_(num_options)
{
    voter_ids_.reserve(num_voters_);
    tokens_.reserve(num_voters_);

    options_.resize(num_voters_);
    ip_addrs_.resize(num_voters_);

    hashes_.resize(num_voters_);
}


VoteData::~VoteData()
{
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
    for (int i = 0; i < num_options_; i++) {
        options_in >> options_[i];
        assert(!options_in.eof());
    }
    options_in.close();
}


void VoteData::readIPsFromFile(const std::string& filename /*= IP_FILE*/)
{
    std::ifstream ips_in(filename);
    for (int i = 0; i < num_options_; i++) {
        ips_in >> ip_addrs_[i];
        assert(!ips_in.eof());
    } 
    ips_in.close();
}


void VoteData::processHashes(char** hashes, int sender_index)
{
    for (int i = 0; i < num_voters_; i++) {
        if (!validateHash(hashes[i], i)) {
            auto vote = requestVote(sender_index, i);
            auto key = requestKey(sender_index, i);
            if (verifyVote(vote, i) && verifyKey(key, i)) {
                writeVote(vote, i);
                writeKey(key, i);
            }
            else
                addBadHash(hashes[i]);                 
        }
    }
}


bool VoteData::validateHash(char hash[32], int i)
{
    if (badHash(hash))
        return false;

    for (int ch = 0; ch < 32; ch++) {
        if (hash[ch] != hashes_[i][ch]) {
            return false;
        }
    }

    return true;
}


void VoteData::addBadHash(char bad_hash[32])
{
    std::array<char, 32> hash;
    for (int i = 0; i < 32; i++)
        hash[i] = bad_hash[i];

    bad_hashes_.insert(hash);
}


bool VoteData::badHash(char hash[32])
{
    std::array<char, 32> h;
    for (int i = 0; i < 32; i++)
        h[i] = hash[i];

    return (bad_hashes_.count(h) > 0);
}


void VoteData::setVerifier(const ECGroup& ecg, 
                           const CryptoPP::ECPPoint& generator)
{
    CryptoPP::ECPPoint id_sum;
    std::vector<CryptoPP::ECPPoint> token_sums(num_options_);
    for (int i = 0; i < num_voters_; i++) {
        id_sum = ecg.curve.Add(id_sum, voter_ids_[i]);
        for (int option = 0; option < num_options_; option++) {
            token_sums[option] = ecg.curve.Add(token_sums[option],
                                               tokens_[i][option]);
        }
    }

    verifier_ = new Verifier(ecg, generator, id_sum, token_sums);
}


Vote VoteData::requestVote(int sender_index, int vote_index)
{
    Vote vote; // ask ip_addrs[sender_index] for vote[vote_index]
    return vote;
}


Key VoteData::requestKey(int sender_index, int key_index)
{
    Key key; // ask ip_addrs[sender_index] for key[key_index]
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

    std::ofstream key_out(filename);
    for (int i = 0; i < num_options_; i++) {
        key_out << key.value(i).x << " " << key.value(i).y << std::endl;
    }
    key_out.close();
}


void VoteData::writeHash(const Vote& vote, const Key& key, int index)
{
    std::string hash_data = vote.getHashData();
    hash_data += key.getHashData();

    HashTo32(hash_data, hashes_[index].data());
}


void HashTo32(const std::string& hash_data, char output[32])
{    
    CryptoPP::SHA3_256 hash;
    hash.Update((CryptoPP::byte*)hash_data.data(), hash_data.size());
    hash.TruncatedFinal((CryptoPP::byte*)output, 32);
}