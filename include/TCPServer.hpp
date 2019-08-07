#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP


#include "VoteData.hpp"
#include "TCPConnection.hpp"


class TCPServer {
public:
    TCPServer(VoteData& vote_data, 
              boost::asio::io_context& io_context);

private:
    VoteData& vote_data_;
    boost::asio::io_context& io_;
    boost::asio::ip::tcp::acceptor acceptor_;

    void startAccept();
    void handleAccept(boost::shared_ptr<ServerConnection> connection,
                      const boost::system::error_code& error);
};


#endif