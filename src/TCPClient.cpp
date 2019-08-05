#include "TCPClient.hpp"
#include "VoteData.hpp"


using namespace boost::asio::ip;


TCPClient::TCPClient(VoteData& vote_data,
                     boost::asio::io_context& io_context)
                     :
                     vote_data_(vote_data),
                     io_(io_context),
                     resolver_(io_context)
{
    srand(static_cast<unsigned int>(time(nullptr)));
    startConnect();
}


void TCPClient::startConnect()
{
    int num_voters = vote_data_.numVoters();
    bool* received = new bool[num_voters];

    while (true) {
        int ip_index = rand() % num_voters;
        auto endpoints = resolver_.resolve(vote_data_.ip(ip_index), 
                                           std::to_string(PORT));
        auto new_connection = TCPConnection::create(io_, vote_data_);
        boost::system::error_code ec;
        boost::asio::connect(new_connection->socket(), endpoints, ec);

        if (!ec) {
            for (int i = 0; i < num_voters; ++i)
                received[i] = false;
            boost::asio::read(new_connection->socket(), 
                              boost::asio::buffer(received, num_voters));
            int index = vote_data_.processReceived(received);
            new_connection->sendVKPair(index);
        }
    }

    delete [] received;
}