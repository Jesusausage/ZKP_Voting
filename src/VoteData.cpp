#include "VoteData.hpp"


VoteData::VoteData(const ECGroup& ecg,
                   const CryptoPP::ECPPoint& generator,
                   const PublicData& pub_data, 
                   const PrivateData& priv_data)
                   :
                   ecg_(ecg),
                   gen_(generator),
                   pub_(pub_data),
                   priv_(priv_data)
{
    received_ = new bool[pub_data.numVoters()];
    for (int i = 0; i < pub_data.numVoters(); ++i)
        received_[i] = false;

    verifier_ = new Verifier(ecg_, gen_, pub_.idSum(), pub_.tokenSums());

    findVoterIndex();
    checkExistingVotes();
    if (received_[voter_index_] == false)
        getUserVote();
}


VoteData::~VoteData()
{
    delete verifier_;

    delete [] received_;
}


int VoteData::processReceived(bool received[])
{
    for (int i = 0; i < pub_.numVoters(); ++i) {
        if (received[i] == false &&
            received_[i] == true) {
            return i;
        }
    }

    return -1;
}


void VoteData::processVKPair(CryptoPP::byte* input, int index)
{
    size_t vote_length = 326 * pub_.numOptions();

    Vote vote(input, pub_.numOptions(), ecg_.curve);
    Key key(input + vote_length, pub_.numOptions(), ecg_.curve);

    if (verifyVote(vote, index) && verifyKey(key, index)) {
        writeVote(vote, index);
        writeKey(key, index);
        received_[index] = true;
        checkComplete();
    }
}


void VoteData::checkComplete() 
{
    bool complete = true;
    for (int i = 0; i < numVoters(); ++i) {
        if (!received_[i])
            complete = false;
    }

    if (complete) {
        std::cout << "All votes received. Tallying..." << std::endl;
        tallyVotes();
    }
}


void VoteData::tallyVotes()
{
    int num_opt = pub_.numOptions();

    std::string filename;
    std::fstream fin;
    size_t vote_length = 326 * num_opt;
    size_t key_length = 163 * num_opt;

    std::vector<CryptoPP::ECPPoint> alpha(num_opt);
    std::vector<CryptoPP::ECPPoint> beta(num_opt);

    auto* vote = new CryptoPP::byte[vote_length];
    auto* key = new CryptoPP::byte[key_length];
    for (int i = 0; i < pub_.numVoters(); ++i) {   
        filename = VOTE_FILE;
        filename += std::to_string(i);
        fin.open(filename, std::ios::in | std::ios::binary);
        if (fin.is_open()) {
            fin.read((char*)vote, vote_length);
            Vote v(vote, num_opt, ecg_.curve);
            for (int opt = 0; opt < num_opt; ++opt)
                beta[opt] = ecg_.curve.Add(beta[opt], v.value(opt));
        }
        fin.close();

        filename = KEY_FILE;
        filename += std::to_string(i);
        fin.open(filename, std::ios::in | std::ios::binary);
        if (fin.is_open()) {
            fin.read((char*)key, key_length);
            Key k(key, num_opt, ecg_.curve);
            for (int opt = 0; opt < num_opt; ++opt) 
                alpha[opt] = ecg_.curve.Add(alpha[opt], k.value(opt));
        }
        fin.close();
    }

    std::fstream result("results.txt", std::ios::out);
    for (int opt = 0; opt < num_opt; ++opt) {
        auto gm = ecg_.curve.Add(ecg_.curve.Inverse(alpha[opt]), beta[opt]);

        int m = 0;
        while (!(gm == ecg_.curve.Multiply(m, gen_)))
            m += 1;
        std::cout << pub_.option(opt) << ": " << m << std::endl;
        result << pub_.option(opt) << ": " << m << std::endl;
    }
}


std::string VoteData::randomIP() const
{
    int index;
    do
        index = rand() % pub_.numVoters();
    while (index == voter_index_);

    return pub_.ip(index);
}


void VoteData::checkExistingVotes() 
{
    std::string filename;
    std::fstream fin;
    size_t vote_length = 326 * pub_.numOptions();
    size_t key_length = 163 * pub_.numOptions();

    auto* vote = new CryptoPP::byte[vote_length];
    auto* key = new CryptoPP::byte[key_length];
    for (int i = 0; i < pub_.numVoters(); ++i) {     
        filename = VOTE_FILE;
        filename += std::to_string(i);
        fin.open(filename, std::ios::in | std::ios::binary);
        if (fin.is_open()) {
            fin.read((char*)vote, vote_length);
            Vote v(vote, pub_.numOptions(), ecg_.curve);
            if (verifyVote(v, i))
                received_[i] = true;
        }
        fin.close();

        filename = KEY_FILE;
        filename += std::to_string(i);
        fin.open(filename, std::ios::in | std::ios::binary);
        if (fin.is_open()) {
            fin.read((char*)key, key_length);
            Key k(key, pub_.numOptions(), ecg_.curve);
            if (!verifyKey(k, i))
                received_[i] = false;
        }
        fin.close();
    }
}


void VoteData::getUserVote()
{
    Voter voter(ecg_, gen_, pub_.idSum(), pub_.tokens(voter_index_));
    voter.setTokenKeys(priv_.tokenKeys());
    voter.castVote(getUserInput());
    Vote vote = voter.getVoteAndProofs();

    KeyGen keygen(ecg_, gen_, pub_.tokenSums(), pub_.id(voter_index_));
    keygen.setIDKey(priv_.idKey());
    Key key = keygen.getKeysAndProofs();

    writeVote(vote, voter_index_);
    writeKey(key, voter_index_);
    received_[voter_index_] = true;
}


int VoteData::getUserInput()
{
    std::cout << "Options: " << std::endl;
    for (int i = 0; i < pub_.numOptions(); ++i) {
        std::cout << i << ": " << pub_.option(i) << std::endl;
    }
    std::cout << std::endl << "Select an option to vote for: ";

    int option;
    std::cin >> option;
    while (option < 0 || option >= pub_.numOptions()) {
        std::cout << "Please choose a valid option: ";
        std::cin >> option;
    }

    std::cout << "You have voted for " << pub_.option(option) << std::endl;
    return option;
}


bool VoteData::verifyVote(const Vote& vote, int index)
{
    verifier_->setTokens(pub_.tokens(index)); 
    return verifier_->verifyVote(vote);
}


bool VoteData::verifyKey(const Key& key, int index)
{
    verifier_->setID(pub_.id(index));
    return verifier_->verifyKey(key);
}


void VoteData::writeVote(const Vote& vote, int index)
{
    std::string filename = VOTE_FILE;
    filename += std::to_string(index);

    size_t length = 326 * pub_.numOptions();
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

    size_t length = 163 * pub_.numOptions();
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


void VoteData::findVoterIndex()
{
    auto id = ecg_.curve.Multiply(priv_.idKey(), gen_);
    int i = 0;
    while (i < pub_.numVoters() && !(id == pub_.id(i)))
        i++;

    if (i == pub_.numVoters()) {
        std::cerr << "Invalid private key." << std::endl;
        exit(INVALID_PRIV_KEY);
    }
    else
        voter_index_ = i;

    const auto& tokens = pub_.tokens(voter_index_);
    const auto& token_keys = priv_.tokenKeys();
    for (int k = 0; k < pub_.numOptions(); k++) {
        if (!(tokens[k] == ecg_.curve.Multiply(token_keys[k], gen_))) {
            std::cerr << "Invalid private key." << std::endl;
            exit(INVALID_PRIV_KEY);
        }
    }
 }