#ifndef TCP_CLIENT_HPP
#define TCP_CLIENT_HPP


#include <boost/asio.hpp>
#include <chrono>


class VoteData;


class TCPClient {
public:
    TCPClient(const VoteData& vote_data, boost::asio::io_context& io_context);

    int receiveRequest(boost::asio::ip::tcp::socket& sock);

private:
    const VoteData& vote_data_;
    boost::asio::io_context& io_context_;
};


#endif