#include "VoteData.hpp"


VoteData::VoteData(const ECGroup& ecg,
                   const CryptoPP::ECPPoint& generator,
                   int num_voters, int num_options)
                   :
                   ecg_(ecg),
                   gen_(generator),
                   num_voters_(num_voters), 
                   num_options_(num_options)
{
    voter_ids_.reserve(num_voters_);
    tokens_.reserve(num_voters_);

    options_.resize(num_voters_);
    ip_addrs_.resize(num_voters_);

    hashes_ = new CryptoPP::byte[num_voters_ * 32];

    readTokensFromFile();
    readIDsFromFile();
    readOptionsFromFile();
    readIPsFromFile();
    setVerifier();
}


VoteData::~VoteData()
{
    if (verifier_)
        delete verifier_;

    delete [] hashes_;
}


void VoteData::readTokensFromFile()
{
    std::ifstream token_in(TOKEN_FILE);
    if (!token_in.is_open()) {
        std::cerr << "Error opening tokens file." << std::endl;
        exit(FILE_OPENING_ERROR);
    }
    ReadTokens(token_in, tokens_, num_options_);
    token_in.close();
}


void VoteData::readIDsFromFile()
{
    std::ifstream id_in(ID_FILE);
    if (!id_in.is_open()) {
        std::cerr << "Error opening tokens file." << std::endl;
        exit(FILE_OPENING_ERROR);
    }
    ReadIDs(id_in, voter_ids_);
    id_in.close();
}


void VoteData::readOptionsFromFile()
{
    std::ifstream options_in(OPTION_FILE);
    if (!options_in.is_open()) {
        std::cerr << "Error opening tokens file." << std::endl;
        exit(FILE_OPENING_ERROR);
    }
    for (int i = 0; i < num_options_; i++) {
        options_in >> options_[i];
        assert(!options_in.eof());
    }
    options_in.close();
}


void VoteData::readIPsFromFile()
{
    std::ifstream ips_in(IP_FILE);
    if (!ips_in.is_open()) {
        std::cerr << "Error opening tokens file." << std::endl;
        exit(FILE_OPENING_ERROR);
    }
    for (int i = 0; i < num_options_; i++) {
        ips_in >> ip_addrs_[i];
        assert(!ips_in.eof());
    } 
    ips_in.close();
}


void VoteData::processHashes(CryptoPP::byte* hashes, int sender_index)
{
    for (int i = 0; i < num_voters_; i++) {
        int offset = 32 * i;
        if (!validateHash(hashes + offset, i)) {
            auto msg = makeRequestMsg(i);
            // SEND MSG TO sender_index
        }
    }
}


void VoteData::processVKPair(CryptoPP::byte* input, int index)
{
    size_t vote_length = 326 * num_options_;

    Vote vote(input, num_options_, ecg_.curve);
    Key key(input + vote_length, num_options_, ecg_.curve);

    if (verifyVote(vote, index) && verifyKey(key, index)) {
        writeVote(vote, index);
        writeKey(key, index);
        writeHash(vote, key, index);
    }
    else {
        addBadHash(vote, key);
    }
}


boost::asio::const_buffer VoteData::makeHashesMsg()
{
    size_t length = 32 * num_voters_;
    return boost::asio::const_buffer(hashes_, length);
}


boost::asio::const_buffer VoteData::makeRequestMsg(int index)
{
    int req[1] = { index };
    return boost::asio::const_buffer(req, 1);
}


boost::asio::const_buffer VoteData::makeVKPairMsg(int index)
{
    size_t length = 489 * num_options_;
    CryptoPP::byte* output = new CryptoPP::byte[length];

    readVote(index, output, num_options_);
    size_t offset = 326 * num_options_;
    readKey(index, output + offset, num_options_);
    auto ret = boost::asio::const_buffer(output, length);

    delete [] output;
    return ret;
}


bool VoteData::validateHash(CryptoPP::byte hash[32], int i)
{
    if (badHash(hash))
        return false;

    int offset = 32 * i;
    for (int ch = 0; ch < 32; ch++) {
        if (hash[ch] != hashes_[offset + ch]) {
            return false;
        }
    }

    return true;
}


void VoteData::addBadHash(const Vote& vote, const Key& key)
{
    std::string hash_data = vote.getHashData();
    hash_data += key.getHashData();

    std::array<CryptoPP::byte, 32> hash;
    hashTo32(hash_data, hash.data()); 

    bad_hashes_.insert(hash);
}


bool VoteData::badHash(CryptoPP::byte hash[32])
{
    std::array<CryptoPP::byte, 32> h;
    for (int i = 0; i < 32; i++) {
        h[i] = hash[i];
    }

    return (bad_hashes_.count(h) > 0);
}


void VoteData::setVerifier()
{
    CryptoPP::ECPPoint id_sum;
    std::vector<CryptoPP::ECPPoint> token_sums(num_options_);
    for (int i = 0; i < num_voters_; i++) {
        id_sum = ecg_.curve.Add(id_sum, voter_ids_[i]);
        for (int option = 0; option < num_options_; option++) {
            token_sums[option] = ecg_.curve.Add(token_sums[option],
                                               tokens_[i][option]);
        }
    }

    verifier_ = new Verifier(ecg_, gen_, id_sum, token_sums);
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

    size_t length = 326 * num_options_;
    char* o = new char[length];
    int n;
    vote.serialise((CryptoPP::byte*)o, n);

    std::fstream vote_out(filename, std::ios::out | std::ios::binary);
    vote_out.write(o, length);
    vote_out.close();

    delete [] o;
}


void VoteData::writeKey(const Key& key, int index)
{
    std::string filename = KEY_FILE;
    filename += std::to_string(index);

    size_t length = 163 * num_options_;
    char* o = new char[length];
    int n;
    key.serialise((CryptoPP::byte*)o, n);

    std::fstream key_out(filename, std::ios::out | std::ios::binary);
    key_out.write(o, length);
    key_out.close();

    delete [] o;
}


void VoteData::readVote(int index, CryptoPP::byte* output, int num_options) 
{
    std::string filename = VOTE_FILE;
    filename += std::to_string(index);

    std::fstream vote_in(filename, std::ios::in | std::ios::binary);
    vote_in.read((char*)output, 326 * num_options);
    vote_in.close();
}


void VoteData::readKey(int index, CryptoPP::byte* output, int num_options) 
{
    std::string filename = KEY_FILE;
    filename += std::to_string(index);

    std::fstream key_in(filename, std::ios::in | std::ios::binary);
    key_in.read((char*)output, 163 * num_options);
    key_in.close();
}


void VoteData::writeHash(const Vote& vote, const Key& key, int index)
{
    std::string hash_data = vote.getHashData();
    hash_data += key.getHashData();

    int offset = 32 * index;
    hashTo32(hash_data, hashes_ + offset);
}


void VoteData::hashTo32(const std::string& hash_data, CryptoPP::byte output[32])
{    
    CryptoPP::SHA3_256 hash;
    hash.Update((CryptoPP::byte*)hash_data.data(), hash_data.size());
    hash.TruncatedFinal(output, 32);
}