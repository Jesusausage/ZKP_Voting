#include "VoteData.hpp"


VoteData::VoteData(const ECGroup& ecg,
                   const CryptoPP::ECPPoint& generator,
                   int num_voters, 
                   int num_options)
                   :
                   ecg_(ecg),
                   gen_(generator),
                   pub_(ecg, num_voters, num_options),
                   priv_(num_options)
{
    received_ = new bool[num_voters];
    for (int i = 0; i < num_voters; ++i)
        received_[i] = false;

    verifier_ = new Verifier(ecg_, gen_, pub_.idSum(), pub_.tokenSums());

    findVoterIndex();
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


// void VoteData::getUserVote()
// {
//     Voter voter(ecg_, gen_, id_sum_, tokens_[voter_index_]);
//     voter.setTokenKeys(token_keys_);
//     voter.castVote(getUserInput());
//     Vote vote = voter.getVoteAndProofs();

//     KeyGen keygen(ecg_, gen_, token_sums_, voter_ids_[voter_index_]);
//     keygen.setIDKey(id_key_);
//     Key key = keygen.getKeysAndProofs();

//     if (!verifyVote(vote, voter_index_) || !verifyKey(key, voter_index_)) {
//         std::cerr << "Private keys invalid." << std::endl;
//         exit(INVALID_PRIV_KEY);
//     }
//     writeVote(vote, voter_index_);
//     writeKey(key, voter_index_);
//     received_[voter_index_] = true;
// }


// int VoteData::getUserInput()
// {
//     std::cout << "Options: " << std::endl;
//     for (int i = 0; i < pub_.numOptions(); ++i) {
//         std::cout << i << ": " << options_[i] << std::endl;
//     }
//     std::cout << std::endl << "Select an option to vote for: ";

//     int option;
//     std::cin >> option;
//     while (option < 0 || option >= pub_.numOptions()) {
//         std::cout << "Please choose a valid option: ";
//         std::cin >> option;
//     }

//     std::cout << "You have voted for " << options_[option] << std::endl;
//     return option;
// }


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