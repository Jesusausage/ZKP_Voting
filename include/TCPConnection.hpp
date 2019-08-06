#ifndef TCP_CONNECTION_HPP
#define TCP_CONNECTION_HPP


#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <cryptopp/cryptlib.h>


#define PORT 1337


class VoteData;


class TCPConnection : public boost::enable_shared_from_this<TCPConnection> {
public:
    static boost::shared_ptr<TCPConnection> create(boost::asio::io_context& io_context,
                                                   VoteData& vote_data);
    void sendReceived();
    void sendVKPair(int index);
    inline boost::asio::ip::tcp::socket& socket()
        { return socket_; }

private:
    boost::asio::ip::tcp::socket socket_;
    VoteData& vote_data_;

    bool* received_msg_;
    int num_voters_;
    
    std::vector<CryptoPP::byte> vkpair_msg_;

    TCPConnection(boost::asio::io_context& io_context, 
                  VoteData& vote_data);

    void makeReceivedMsg();
    void makeVKPairMsg(int index);

    void handleWriteReceived(const boost::system::error_code& /*error*/,
                             size_t /*bytes_transferred*/);
    void handleWriteVKPair(const boost::system::error_code& /*error*/,
                           size_t /*bytes_transferred*/);
};


/* ====================================================================== */


int ByteToInt(const CryptoPP::byte ch[4]);
void IntToByte(int n, CryptoPP::byte output[4]);


#endif