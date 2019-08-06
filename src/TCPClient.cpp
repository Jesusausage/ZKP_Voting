#include "TCPClient.hpp"
#include "VoteData.hpp"


using namespace boost::asio::ip;


TCPClient::TCPClient(VoteData& vote_data,
                     boost::asio::io_context& io_context)
                     :
                     vote_data_(vote_data),
                     io_(io_context)
{
    startConnect();
}


void TCPClient::startConnect()
{
    boost::shared_ptr<tcp::endpoint> endpoint(
        new tcp::endpoint(address::from_string(vote_data_.randomIP()), PORT));
    auto connection = TCPConnection::create(io_, vote_data_);
    connection->socket().async_connect(*endpoint, 
                                       boost::bind(&TCPClient::handleConnect,
                                                   this,
                                                   connection,
                                                   boost::asio::placeholders::error));
}


void TCPClient::handleConnect(boost::shared_ptr<TCPConnection> connection,
                              const boost::system::error_code& error)
{
    if (!error) {
        int num_voters = vote_data_.numVoters();
        bool* received = new bool[num_voters];
        for (int i = 0; i < num_voters; ++i)
            received[i] = false;

        boost::asio::read(connection->socket(), 
                          boost::asio::buffer(received, num_voters));
        int index = vote_data_.processReceived(received);
        connection->sendVKPair(index);

        delete [] received;
    }

    startConnect();
}