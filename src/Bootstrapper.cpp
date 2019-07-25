#include "Bootstrapper.hpp"


Bootstrapper::Bootstrapper(int num_voters, int num_options)
                           :
                           num_voters_(num_voters), 
                           num_options_(num_options)
{
    voter_ids_ = new CryptoPP::ECPPoint[num_voters_];

    tokens_ = new CryptoPP::ECPPoint*[num_voters_];
    for (int i = 0; i < num_voters_; i++)
        tokens_[i] = new CryptoPP::ECPPoint[num_options_];

    options_ = new std::string[num_options_];

    ip_addrs_ = new std::string[num_voters_];
}


Bootstrapper::~Bootstrapper()
{
    delete [] voter_ids_;
    for (int i = 0; i < num_voters_; i++)
        delete [] tokens_[i];
    delete [] tokens_;

    delete [] options_;
    delete [] ip_addrs_;
}


void Bootstrapper::readTokensFromFile(const std::string filename)
{
    std::ifstream token_in(filename);
    ReadTokens(token_in, tokens_, num_options_);
    token_in.close();
}


void Bootstrapper::readIDsFromFile(const std::string filename)
{
    std::ifstream id_in(filename);
    ReadIDs(id_in, voter_ids_);
    id_in.close();
}


void Bootstrapper::readOptionsFromFile(const std::string filename)
{
    std::ifstream options_in(filename);
    int i = 0;

    do
        options_in >> options_[i++];
    while (!options_in.eof());
}


void Bootstrapper::run()
{
    using namespace boost::asio;

    io_context io_context;
    ip::tcp::acceptor acceptor(io_context, ip::tcp::endpoint(ip::tcp::v4(), 1337));

    // while (true) {
    //     ip::tcp::socket socket(io_context);
    //     acceptor.accept(socket);

    //     auto ecg = GenerateECGroup();
    //     auto gen = ecg.base;
    //     auto id_key = CryptoPP::Integer(27);
    //     auto id = ecg.curve.Multiply(id_key, gen);
    //     std::vector<CryptoPP::ECPPoint> token_sums;
    //     for (int i = 0; i < 10; i++) {
    //         token_sums.push_back(ecg.curve.Multiply(42 + i, gen));
    //     }

    //     KeyGen key_gen(ecg, gen, token_sums, id);
    //     key_gen.setIDKey(id_key);
    //     Key key = key_gen.getKeysAndProofs();
    //     CryptoPP::byte msg[1630];
    //     int opts[1];
    //     key.serialise(msg, opts[0]);        

    //     std::cout << "running" << std::endl;

    //     boost::system::error_code ignored_error;
    //     boost::asio::write(socket, buffer(opts), ignored_error);
    //     boost::asio::write(socket, buffer(msg), ignored_error);
    // }
}