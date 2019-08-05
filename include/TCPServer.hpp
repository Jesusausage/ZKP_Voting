#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP


#include "TCPConnection.hpp"


class VoteData;


class TCPServer {
public:
    TCPServer(VoteData& vote_data, 
              boost::asio::io_context& io_context);

private:
    VoteData& vote_data_;
    boost::asio::io_context& io_;
    boost::asio::ip::tcp::acceptor acceptor_;

    void startAccept();
    void handleAccept(boost::shared_ptr<TCPConnection> new_connection,
                      const boost::system::error_code& error);
};


#endif