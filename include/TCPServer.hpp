#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP


#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <cryptopp/cryptlib.h>


class TCPConnection;
class VoteData;


class TCPServer {
public:
    TCPServer(const VoteData& vote_data, 
              boost::asio::io_context& io_context);
              
    boost::asio::const_buffer makeReceivedMessage();
    boost::asio::const_buffer makeVKMessage(int index);
    int numOptions();

    int waitForResponse(boost::asio::ip::tcp::socket& sock);

private:
    const VoteData& vote_data_;
    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;

    void startAccept();
    void handleAccept(boost::shared_ptr<TCPConnection> new_connection,
                      const boost::system::error_code& error);
};


/* ====================================================================== */


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

    void handleWrite(const boost::system::error_code& /*error*/,
                     size_t /*bytes_transferred*/);
};


/* ====================================================================== */


int byteToInt(const CryptoPP::byte ch[4]);
void intToByte(int n, CryptoPP::byte output[4]);


#endif