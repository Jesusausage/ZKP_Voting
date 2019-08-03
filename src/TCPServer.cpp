#include "TCPServer.hpp"
#include "TCPClient.hpp"
#include "VoteData.hpp"


using namespace boost::asio::ip;


TCPServer::TCPServer(const VoteData& vote_data,
                     TCPClient* client,
                     boost::asio::io_context& io_context)
                     :
                     vote_data_(vote_data),
                     client_(client),
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


boost::asio::const_buffer TCPServer::makeHashesMessage()
{
    return vote_data_.makeHashesMsg();
}


boost::asio::const_buffer TCPServer::makeVKMessage(int index)
{
    return vote_data_.makeVKPairMsg(index);
}


int TCPServer::waitForClientRequest(tcp::socket& sock)
{
    return client_->receiveRequest(sock);
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
    boost::asio::async_write(socket_, server_->makeHashesMessage(),
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
    int request = server_->waitForClientRequest(socket_);

    boost::asio::async_write(socket_, server_->makeVKMessage(request), 
                             boost::bind(&TCPConnection::handleWrite,
                                         shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}