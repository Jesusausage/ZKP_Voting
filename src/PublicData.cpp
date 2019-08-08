#include "PublicData.hpp"


PublicData::PublicData(const ECGroup& ecg, int num_voters, int num_options)
                       :
                       num_voters_(num_voters), num_options_(num_options)
{
    tokens_.reserve(num_voters);
    voter_ids_.reserve(num_voters);
    readTokensFromFile();
    readIDsFromFile();
    setSums(ecg);

    options_.resize(num_options);
    ip_addrs_.resize(num_voters);
    readOptionsFromFile();
    readIPsFromFile();
}


void PublicData::readTokensFromFile()
{
    std::ifstream token_in(TOKEN_FILE);
    if (!token_in.is_open()) {
        std::cerr << "Error opening " << TOKEN_FILE << std::endl;
        exit(FILE_OPENING_ERROR);
    }
    ReadTokens(token_in, tokens_, num_options_);
    token_in.close();
}


void PublicData::readIDsFromFile()
{
    std::ifstream id_in(ID_FILE);
    if (!id_in.is_open()) {
        std::cerr << "Error opening " << ID_FILE << std::endl;
        exit(FILE_OPENING_ERROR);
    }
    ReadIDs(id_in, voter_ids_);
    id_in.close();
}


void PublicData::setSums(const ECGroup& ecg)
{
    token_sums_.resize(num_options_);

    for (int i = 0; i < num_voters_; i++) {
        id_sum_ = ecg.curve.Add(id_sum_, voter_ids_[i]);
        for (int option = 0; option < num_options_; option++) {
            token_sums_[option] = ecg.curve.Add(token_sums_[option],
                                                 tokens_[i][option]);
        }
    }
}


void PublicData::readOptionsFromFile()
{
    std::ifstream options_in(OPTION_FILE);
    if (!options_in.is_open()) {
        std::cerr << "Error opening " << OPTION_FILE << std::endl;
        exit(FILE_OPENING_ERROR);
    }
    for (int i = 0; i < num_options_; i++) {
        if (options_in.eof()) {
            std::cerr << "Incomplete options file: " << OPTION_FILE << std::endl;
            exit(INCOMPLETE_FILE_ERROR);
        }
        options_in >> options_[i];        
    }
    options_in.close();
}


void PublicData::readIPsFromFile()
{
    std::ifstream ips_in(IP_FILE);
    if (!ips_in.is_open()) {
        std::cerr << "Error opening " << IP_FILE << std::endl;
        exit(FILE_OPENING_ERROR);
    }
    for (int i = 0; i < num_voters_; i++) {
        if (ips_in.eof()) {
            std::cerr << "Incomplete IP file: " << IP_FILE << std::endl;
            exit(INCOMPLETE_FILE_ERROR);
        }
        ips_in >> ip_addrs_[i];
    }
    ips_in.close();
}