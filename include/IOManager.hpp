#ifndef IO_MANAGER_HPP
#define IO_MANAGER_HPP


#include "VoteData.hpp"
#include <boost/bind.hpp>
#include <boost/asio.hpp>


class IOManager {
public:
    IOManager(int num_voters, int num_options);

    void sendVote(int index);
    void sendKey(int index);

private:
    const int num_voters_;
    const int num_options_;

    boost::asio::io_context io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
};


#endif