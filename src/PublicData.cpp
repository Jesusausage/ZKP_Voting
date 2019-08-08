#include "PublicData.hpp"


PublicData::PublicData(int num_voters, int num_options)
{
    tokens_.reserve(num_voters);
    voter_ids_.reserve(num_voters);
    readTokensFromFile(num_options);
    readIDsFromFile();

    options_.resize(num_options);
    ip_addrs_.resize(num_voters);
    readOptionsFromFile(num_options);
    readIPsFromFile(num_voters);
}


void PublicData::readTokensFromFile(int num_options)
{
    std::ifstream token_in(TOKEN_FILE);
    if (!token_in.is_open()) {
        std::cerr << "Error opening " << TOKEN_FILE << std::endl;
        exit(FILE_OPENING_ERROR);
    }
    ReadTokens(token_in, tokens_, num_options);
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


void PublicData::readOptionsFromFile(int num_options)
{
    std::ifstream options_in(OPTION_FILE);
    if (!options_in.is_open()) {
        std::cerr << "Error opening " << OPTION_FILE << std::endl;
        exit(FILE_OPENING_ERROR);
    }
    for (int i = 0; i < num_options; i++) {
        if (options_in.eof()) {
            std::cerr << "Incomplete options file: " << OPTION_FILE << std::endl;
            exit(INCOMPLETE_FILE_ERROR);
        }
        options_in >> options_[i];        
    }
    options_in.close();
}


void PublicData::readIPsFromFile(int num_voters)
{
    std::ifstream ips_in(IP_FILE);
    if (!ips_in.is_open()) {
        std::cerr << "Error opening " << IP_FILE << std::endl;
        exit(FILE_OPENING_ERROR);
    }
    for (int i = 0; i < num_voters; i++) {
        if (ips_in.eof()) {
            std::cerr << "Incomplete IP file: " << IP_FILE << std::endl;
            exit(INCOMPLETE_FILE_ERROR);
        }
        ips_in >> ip_addrs_[i];
    }
    ips_in.close();
}