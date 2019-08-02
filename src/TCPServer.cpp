#include "TCPServer.hpp"


using namespace boost::asio::ip;


TCPServer::TCPServer(boost::asio::io_context& io_context,
                     CryptoPP::byte** hashes, size_t hashes_size)
                     :
                     io_context_(io_context),
                     acceptor_(io_context, tcp::endpoint(tcp::v4(), 1337)),
                     hashes_(hashes),
                     hashes_size_(hashes_size)
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


boost::asio::mutable_buffer TCPServer::makeMessage()
{
    return boost::asio::buffer(hashes_, hashes_size_);
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
    // CONNECTION ESTABLISHED, DO STUFF HERE

    // boost::asio::async_write(socket_, server_->makeMessage(),
    //                          boost::bind(&TCPConnection::handleWrite, 
    //                                      shared_from_this()));
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


void TCPConnection::handleWrite()
{}