#include "PrivateData.hpp"


PrivateData::PrivateData(const ECGroup& ecg, 
                         const CryptoPP::ECPPoint& generator,
                         const PublicData& pub_data)
                         :
                         dat_(pub_data)
{
    readPrivFromFile();
    findVoterIndex(ecg, generator);
}


void PrivateData::readPrivFromFile()
{
    std::fstream priv_in(PRIV_KEY_FILE, std::ios::in);
    if (!priv_in.is_open()) {
        std::cerr << "Error opening " << PRIV_KEY_FILE << std::endl;
        exit(FILE_OPENING_ERROR);
    }
    token_keys_.resize(dat_.numOptions());
    priv_in >> id_key_ >> std::ws;
    for (int i = 0; i < dat_.numOptions(); ++i) {
        if (priv_in.eof()) {
            std::cerr << "Incomplete private keys file: " << PRIV_KEY_FILE << std::endl;
            exit(INCOMPLETE_FILE_ERROR);
        }
        priv_in >> token_keys_[i] >> std::ws;
    }
    priv_in.close();
}


void PrivateData::findVoterIndex(const ECGroup& ecg, 
                                 const CryptoPP::ECPPoint& gen)
{
    auto id = ecg.curve.Multiply(id_key_, gen);
    for (int i = 0; i < dat_.numVoters(); ++i) {
        if (dat_.id(i) == id) {
            voter_index_ = i;
            return;
        }
    }
}