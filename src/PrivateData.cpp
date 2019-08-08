#include "PrivateData.hpp"


PrivateData::PrivateData(int num_options)
{
    std::fstream priv_in(PRIV_KEY_FILE, std::ios::in);
    if (!priv_in.is_open()) {
        std::cerr << "Error opening " << PRIV_KEY_FILE << std::endl;
        exit(FILE_OPENING_ERROR);
    }
    token_keys_.resize(num_options);
    priv_in >> id_key_ >> std::ws;
    for (int i = 0; i < num_options; ++i) {
        if (priv_in.eof()) {
            std::cerr << "Incomplete private keys file: " << PRIV_KEY_FILE << std::endl;
            exit(INCOMPLETE_FILE_ERROR);
        }
        priv_in >> token_keys_[i] >> std::ws;
    }
    priv_in.close();
}