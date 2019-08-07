#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP


#include "VoteData.hpp"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <cryptopp/cryptlib.h>


#define PORT 1337


class TCPConnection {
public:
    inline boost::asio::ip::tcp::socket& socket()
        { return socket_; }

protected:
    boost::asio::ip::tcp::socket socket_;
    VoteData& vote_data_;

    TCPConnection(boost::asio::io_context& io_context, 
                  VoteData& vote_data);
};


class ServerConnection : public TCPConnection, 
                         public boost::enable_shared_from_this<ServerConnection> {
public:
    static boost::shared_ptr<ServerConnection> create(boost::asio::io_context& io_context,
                                                      VoteData& vote_data);
    void sendReceived();

private:
    bool* received_msg_;
    int num_voters_;

    ServerConnection(boost::asio::io_context& io_context,
                     VoteData& vote_data);

    void makeReceivedMsg();    
    void handleWriteReceived(const boost::system::error_code& /*error*/,
                             size_t /*bytes_transferred*/);
};


class ClientConnection : public TCPConnection,
                         public boost::enable_shared_from_this<ClientConnection> {
public:
    static boost::shared_ptr<ClientConnection> create(boost::asio::io_context& io_context,
                                                      VoteData& vote_data);
    void sendVKPair(int index);

private:
    std::vector<CryptoPP::byte> vkpair_msg_;
    
    ClientConnection(boost::asio::io_context& io_context,
                     VoteData& vote_data);
    
    void makeVKPairMsg(int index);    
    void handleWriteVKPair(const boost::system::error_code& /*error*/,
                           size_t /*bytes_transferred*/);
};


int ByteToInt(const CryptoPP::byte ch[4]);
void IntToByte(int n, CryptoPP::byte output[4]);


#endif