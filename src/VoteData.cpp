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
    options_.resize(num_options_);
    ip_addrs_.resize(num_voters_);

    received_ = new bool[num_voters_];
    for (int i = 0; i < num_voters_; ++i)
        received_[i] = false;

    readTokensFromFile();
    readIDsFromFile();
    readOptionsFromFile();
    readIPsFromFile();

    setSums();
    verifier_ = new Verifier(ecg_, gen_, id_sum_, token_sums_);
    getUserVote();
}


VoteData::~VoteData()
{
    delete verifier_;

    delete [] received_;
}


void VoteData::readTokensFromFile()
{
    std::ifstream token_in(TOKEN_FILE);
    if (!token_in.is_open()) {
        std::cerr << "Error opening " << TOKEN_FILE << std::endl;
        exit(FILE_OPENING_ERROR);
    }
    ReadTokens(token_in, tokens_, num_options_);
    token_in.close();
}


void VoteData::readIDsFromFile()
{
    std::ifstream id_in(ID_FILE);
    if (!id_in.is_open()) {
        std::cerr << "Error opening " << ID_FILE << std::endl;
        exit(FILE_OPENING_ERROR);
    }
    ReadIDs(id_in, voter_ids_);
    id_in.close();
}


void VoteData::readOptionsFromFile()
{
    std::ifstream options_in(OPTION_FILE);
    if (!options_in.is_open()) {
        std::cerr << "Error opening " << OPTION_FILE << std::endl;
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
        std::cerr << "Error opening " << IP_FILE << std::endl;
        exit(FILE_OPENING_ERROR);
    }
    for (int i = 0; i < num_voters_; i++) {
        ips_in >> ip_addrs_[i];
        assert(!ips_in.eof());
        std::cout  << i << std::endl;
    }
    ips_in.close();
}


int VoteData::processReceived(bool received[])
{
    for (int i = 0; i < num_voters_; ++i) {
        if (received[i] == true &&
            received_[i] == false)
            return i;
    }

    return -1;
}


void VoteData::processVKPair(CryptoPP::byte* input, int index)
{
    size_t vote_length = 326 * num_options_;

    Vote vote(input, num_options_, ecg_.curve);
    Key key(input + vote_length, num_options_, ecg_.curve);

    if (verifyVote(vote, index) && verifyKey(key, index)) {
        writeVote(vote, index);
        writeKey(key, index);
        received_[index] = true;
    }
}


boost::asio::const_buffer VoteData::makeReceivedMsg() const
{
    return boost::asio::buffer(received_, num_voters_);
}


boost::asio::const_buffer VoteData::makeVKPairMsg(int index) const
{
    if (index < 0) {
        CryptoPP::byte out[4];
        IntToByte(-1, out);
        return boost::asio::const_buffer(out, 4);
    }

    size_t length = 489 * num_options_ + 4;
    CryptoPP::byte* output = new CryptoPP::byte[length];
    
    IntToByte(index, output);
    size_t offset = 4;
    readVote(index, output + offset, num_options_);
    offset += 326 * num_options_;
    readKey(index, output + offset, num_options_);
    auto ret = boost::asio::const_buffer(output, length);

    delete [] output;
    return ret;
}


std::string VoteData::randomIP() const
{
    int index = rand() % ip_addrs_.size();
    while (index == voter_index_)
        index = rand() % ip_addrs_.size();

    return ip_addrs_[index];
}


void VoteData::setSums()
{
    token_sums_.resize(num_options_);

    for (int i = 0; i < num_voters_; i++) {
        id_sum_ = ecg_.curve.Add(id_sum_, voter_ids_[i]);
        for (int option = 0; option < num_options_; option++) {
            token_sums_[option] = ecg_.curve.Add(token_sums_[option],
                                                 tokens_[i][option]);
        }
    }
}


void VoteData::getUserVote()
{
    readPrivFromFile();

    Voter voter(ecg_, gen_, id_sum_, tokens_[voter_index_]);
    voter.setTokenKeys(token_keys_);
    voter.castVote(0);
    Vote vote = voter.getVoteAndProofs();

    KeyGen keygen(ecg_, gen_, token_sums_, voter_ids_[voter_index_]);
    keygen.setIDKey(id_key_);
    Key key = keygen.getKeysAndProofs();

    if (!verifyVote(vote, voter_index_) || !verifyKey(key, voter_index_)) {
        std::cerr << "Private keys invalid." << std::endl;
        exit(INVALID_PRIV_KEY);
    }
    writeVote(vote, voter_index_);
    writeKey(key, voter_index_);
    received_[voter_index_] = true;
}


void VoteData::readPrivFromFile()
{
    std::fstream priv_in(PRIV_KEY_FILE, std::ios::in);
    if (!priv_in.is_open()) {
        std::cerr << "Error opening " << PRIV_KEY_FILE << std::endl;
        exit(FILE_OPENING_ERROR);
    }
    token_keys_.resize(num_options_);
    priv_in >> id_key_ >> std::ws;
    for (int i = 0; i < num_options_; ++i)
        priv_in >> token_keys_[i] >> std::ws;
    priv_in.close();

    auto id = ecg_.curve.Multiply(id_key_, gen_);
    for (int i = 0; i < num_voters_; ++i) {
        if (voter_ids_[i] == id) {
            voter_index_ = i;
            break;
        }
    }
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
    if (!vote_in.is_open()) {
        std::cerr << "Error opening " << filename << std::endl;
        return;
    }
    vote_in.read((char*)output, 326 * num_options);
    vote_in.close();
}


void VoteData::readKey(int index, CryptoPP::byte* output, int num_options) 
{
    std::string filename = KEY_FILE;
    filename += std::to_string(index);

    std::fstream key_in(filename, std::ios::in | std::ios::binary);
    if (!key_in.is_open()) {
        std::cerr << "Error opening " << filename << std::endl;
        return;
    }
    key_in.read((char*)output, 163 * num_options);
    key_in.close();
}