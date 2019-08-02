#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP


#include "VoteData.hpp"
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>


class TCPConnection;


class TCPServer {
public:
    TCPServer(boost::asio::io_context& io_context,
              CryptoPP::byte** hashes, size_t hashes_size);
              
    static enum MsgType { HASHES, REQUEST, VOTE, KEY };
    boost::asio::mutable_buffer makeMessage();

private:
    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;

    CryptoPP::byte** hashes_;
    size_t hashes_size_;

    void startAccept();
    void handleAccept(boost::shared_ptr<TCPConnection> new_connection,
                      const boost::system::error_code& error);
};


class TCPConnection : public boost::enable_shared_from_this<TCPConnection> {
public:
    static boost::shared_ptr<TCPConnection> create(
                            boost::asio::io_context& io_context,
                            TCPServer* server);
    void start();
    boost::asio::ip::tcp::socket& socket();

private:
    boost::asio::ip::tcp::socket socket_;
    TCPServer* server_;

    TCPConnection(boost::asio::io_context& io_context, TCPServer* server);

    void handleWrite();
};


#endif