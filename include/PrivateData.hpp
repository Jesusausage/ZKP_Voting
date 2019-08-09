#ifndef PRIV_DATA_HPP
#define PRIV_DATA_HPP


#include "Errors.hpp"
#include <cryptopp/integer.h>
#include <vector>
#include <fstream>
#include <iostream>


#define PRIV_KEY_FILE "private_keys.txt"


class PrivateData {
public:
    PrivateData(int num_options);

    inline const CryptoPP::Integer& idKey() const
        { return id_key_; }
    inline const std::vector<CryptoPP::Integer>& tokenKeys() const
        { return token_keys_; }

private:
    CryptoPP::Integer id_key_;
    std::vector<CryptoPP::Integer> token_keys_;
};


#endif