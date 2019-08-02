#include "IOManager.hpp"


using namespace boost::asio::ip;


IOManager::IOManager(int num_voters, int num_options)
                     :
                     num_voters_(num_voters),
                     num_options_(num_options),
                     acceptor_(io_context_, tcp::endpoint(tcp::v4(), 1337))
{}


void IOManager::sendVote(int index)
{
    tcp::socket sock(io_context_);
    acceptor_.accept(sock);

    size_t length = 326 * num_options_;
    CryptoPP::byte* output = new CryptoPP::byte[length];
    VoteData::readVote(index, output, num_options_);

    boost::system::error_code ignored_error;
    boost::asio::write(sock, boost::asio::buffer(output, length), ignored_error);

    delete [] output;    
}


void IOManager::sendKey(int index)
{
    tcp::socket sock(io_context_);
    acceptor_.accept(sock);

    size_t length = 163 * num_options_;
    CryptoPP::byte* output = new CryptoPP::byte[length];
    VoteData::readKey(index, output, num_options_);

    boost::system::error_code ignored_error;
    boost::asio::write(sock, boost::asio::buffer(output, length), ignored_error);

    delete [] output;
}