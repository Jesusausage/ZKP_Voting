#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP


#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <cryptopp/cryptlib.h>


#define PORT 1337


class VoteData;


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


class ServerConnection : public TCPConnection {
public:
    ServerConnection(boost::asio::io_context& io_context,
                     VoteData& vote_data);
    void sendReceived();

private:
    bool* received_msg_;
    int num_voters_;

    void makeReceivedMsg();    
    void handleWriteReceived(const boost::system::error_code& /*error*/,
                             size_t /*bytes_transferred*/);
};


class ClientConnection : public TCPConnection {
public:
    ClientConnection(boost::asio::io_context& io_context,
                     VoteData& vote_data);
    void sendVKPair(int index);

private:
    std::vector<CryptoPP::byte> vkpair_msg_;
    
    void makeVKPairMsg(int index);    
    void handleWriteVKPair(const boost::system::error_code& /*error*/,
                           size_t /*bytes_transferred*/);
};


int ByteToInt(const CryptoPP::byte ch[4]);
void IntToByte(int n, CryptoPP::byte output[4]);


#endif