#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP


#include "TCPConnection.hpp"


class VoteData;


class TCPClient {
public:
    TCPClient(VoteData& vote_data, 
              boost::asio::io_context& io_context);

private:
    VoteData& vote_data_;
    boost::asio::io_context& io_;
    boost::asio::ip::tcp::resolver resolver_;

    void startConnect();
};


#endif