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
    TCPServer(const VoteData& vote_data, boost::asio::io_context& io_context);
              
    static enum MsgType { HASHES, REQUEST, VOTE, KEY };
    inline void setMsgType(MsgType msg_type)
        { msg_type_ = msg_type; }
    boost::asio::mutable_buffer makeMessage();

private:
    const VoteData& vote_data_;
    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;

    MsgType msg_type_;

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