#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP


#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <cryptopp/cryptlib.h>


static const int PORT = 1337;


class TCPConnection;
class VoteData;


class TCPServer {
public:
    TCPServer(VoteData& vote_data, 
              boost::asio::io_context& io_context);

    void startServer();
    void startClient();
    void stopClient();

private:
    VoteData& vote_data_;
    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::resolver resolver_;

    bool client_active_ = false;

    void startAccept();
    void handleAccept(boost::shared_ptr<TCPConnection> new_connection,
                      const boost::system::error_code& error);

    void startConnect();
};


/* ====================================================================== */


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

    TCPConnection(boost::asio::io_context& io_context, 
                  VoteData& vote_data);

    void handleWriteReceived(const boost::system::error_code& /*error*/,
                             size_t /*bytes_transferred*/);
    void handleWriteVKPair(const boost::system::error_code& /*error*/,
                           size_t /*bytes_transferred*/);
};


/* ====================================================================== */


int ByteToInt(const CryptoPP::byte ch[4]);
void IntToByte(int n, CryptoPP::byte output[4]);


#endif