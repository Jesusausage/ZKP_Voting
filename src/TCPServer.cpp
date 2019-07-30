#include "TCPServer.hpp"


using namespace boost::asio::ip;


TCPServer::TCPServer(boost::asio::io_context& io_context,
                     std::vector< std::array<char, 32> >& vote_hashes,
                     std::vector< std::array<char, 32> >& key_hashes)
                     :
                     io_context_(io_context),
                     acceptor_(io_context, tcp::endpoint(tcp::v4(), 1337)),
                     vote_hashes_(vote_hashes),
                     key_hashes_(key_hashes)
{}


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


std::vector<char> TCPServer::makeMessage()
{
    size_t msg_size = vote_hashes_.size() * 64;
    std::vector<char> ret;
    ret.reserve(msg_size);

    for (auto hash : vote_hashes_) {
        for (int i = 0; i < 32; i++)
            ret.emplace_back(hash[i]);
    }
    for (auto hash : key_hashes_) {
        for (int i = 0; i < 32; i++)
            ret.emplace_back(hash[i]);
    }

    return ret;
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
    auto msg = server_->makeMessage();

    boost::asio::async_write(socket_, boost::asio::buffer(msg),
                             boost::bind(&TCPConnection::handleWrite, shared_from_this(),
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


void TCPConnection::handleWrite(const boost::system::error_code&, size_t)
{}