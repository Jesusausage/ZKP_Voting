#include "TCPServer.hpp"


using namespace boost::asio::ip;


TCPServer::TCPServer(boost::asio::io_context& io_context)
                     :
                     io_context_(io_context),
                     acceptor_(io_context, tcp::endpoint(tcp::v4(), 1337))
{
    startAccept();
}


void TCPServer::startAccept()
{
    boost::shared_ptr<TCPConnection> new_connection = TCPConnection::create(io_context_);
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


/* ======================================================================== */


boost::shared_ptr<TCPConnection> TCPConnection::create(boost::asio::io_context& io_context)
{
    return boost::shared_ptr<TCPConnection>(new TCPConnection(io_context));
}


void TCPConnection::start()
{
    // async_write some_stuff_ through socket_
    boost::asio::async_write(socket_, boost::asio::buffer(some_stuff_),
                             boost::bind(&TCPConnection::handleWrite, shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}


tcp::socket& TCPConnection::socket()
{
    return socket_;
}


TCPConnection::TCPConnection(boost::asio::io_context& io_context)
                             :
                             socket_(io_context)
{}


void TCPConnection::handleWrite(const boost::system::error_code&, size_t)
{
}