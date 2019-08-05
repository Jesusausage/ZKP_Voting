#include "TCPServer.hpp"
#include "VoteData.hpp"


using namespace boost::asio::ip;


TCPServer::TCPServer(VoteData& vote_data,
                     boost::asio::io_context& io_context)
                     :
                     vote_data_(vote_data),
                     io_(io_context),
                     acceptor_(io_context, tcp::endpoint(tcp::v4(), PORT))
{
    startAccept();
}


void TCPServer::startAccept()
{
    auto new_connection = TCPConnection::create(io_, vote_data_);
    acceptor_.async_accept(new_connection->socket(), 
                           boost::bind(&TCPServer::handleAccept, this,
                                       new_connection,
                                       boost::asio::placeholders::error));
}


void TCPServer::handleAccept(boost::shared_ptr<TCPConnection> new_connection,
                             const boost::system::error_code& error)
{
    if (!error)
        new_connection->sendReceived();
    
    startAccept();
}