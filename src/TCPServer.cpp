#include "TCPServer.hpp"
#include "VoteData.hpp"


using namespace boost::asio::ip;


TCPServer::TCPServer(const VoteData& vote_data,
                     boost::asio::io_context& io_context)
                     :
                     vote_data_(vote_data),
                     io_context_(io_context),
                     acceptor_(io_context, tcp::endpoint(tcp::v4(), 1337))
{
    startAccept();
}


void TCPServer::startAccept()
{
    boost::shared_ptr<TCPConnection> new_connection = TCPConnection::create(io_context_, this);
    acceptor_.async_accept(new_connection->socket(), 
                           boost::bind(&TCPServer::handleAccept, this,
                                       new_connection,
                                       boost::asio::placeholders::error));
}


void TCPServer::handleAccept(boost::shared_ptr<TCPConnection> new_connection,
                             const boost::system::error_code& error)
{
    if (!error)
        new_connection->start();
    
    startAccept();
}


boost::asio::const_buffer TCPServer::makeReceivedMessage()
{
    return vote_data_.makeReceivedMsg();
}


boost::asio::const_buffer TCPServer::makeVKMessage(int index)
{
    return vote_data_.makeVKPairMsg(index);
}


int TCPServer::numOptions()
{
    return vote_data_.numOptions();
}


int TCPServer::waitForResponse(tcp::socket& sock)
{
    return 0;
}


/* ======================================================================== */


boost::shared_ptr<TCPConnection> TCPConnection::create(
                    boost::asio::io_context& io_context,
                    TCPServer* server)
{
    return boost::shared_ptr<TCPConnection>(new TCPConnection(io_context, 
                                                              server));
}


void TCPConnection::start()
{
    boost::asio::async_write(socket_, server_->makeReceivedMessage(),
                             boost::bind(&TCPConnection::handleWrite, 
                                         shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}


tcp::socket& TCPConnection::socket()
{
    return socket_;
}


TCPConnection::TCPConnection(boost::asio::io_context& io_context,
                             TCPServer* server)
                             :
                             socket_(io_context),
                             server_(server)
{}


void TCPConnection::handleWrite(const boost::system::error_code& /*error*/,
                                size_t /*bytes_transferred*/)
{
    CryptoPP::byte index[4];
    boost::asio::read(socket_, boost::asio::buffer(index),
                      boost::asio::transfer_exactly(4));

    size_t length = 489 * server_->numOptions();
    auto* in = new CryptoPP::byte[length];

    boost::asio::read(socket_, boost::asio::buffer(in, length), 
                      boost::asio::transfer_exactly(length));
}


/* ======================================================================== */


int byteToInt(const CryptoPP::byte ch[4])
{
    return (int)ch[0] * (int)16777216 
         + (int)ch[1] * (int)65536
         + (int)ch[2] * (int)256
         + (int)ch[3];
}


void intToByte(int n, CryptoPP::byte output[4])
{
    int first = n / 16777216;
    output[0] = first;

    n %= 16777216;
    int second = n / 65536;
    output[1] = second;

    n %= 65536;
    int third = n % 256;
    output[2] = third;

    n %= 256;
    output[3] = n;
}