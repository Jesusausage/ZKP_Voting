#include "TCPServer.hpp"
#include "VoteData.hpp"


using namespace boost::asio::ip;


TCPServer::TCPServer(VoteData& vote_data,
                     boost::asio::io_context& io_context)
                     :
                     vote_data_(vote_data),
                     io_context_(io_context),
                     acceptor_(io_context, tcp::endpoint(tcp::v4(), PORT)),
                     resolver_(io_context)
{
    srand(static_cast<unsigned int>(time(nullptr)));
}


void TCPServer::startServer()
{
    startAccept();
}


void TCPServer::startClient()
{
    client_active_ = true;
    startConnect();
}


void TCPServer::stopClient()
{
    client_active_ = false;
}


void TCPServer::startAccept()
{
    auto new_connection = TCPConnection::create(io_context_, vote_data_);
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


void TCPServer::startConnect()
{
    int num_voters = vote_data_.numVoters();
    bool* received = new bool[num_voters];

    while (client_active_) {
        int ip_index = rand() % num_voters;
        auto endpoints = resolver_.resolve(vote_data_.ip(ip_index), 
                                           std::to_string(PORT));
        auto new_connection = TCPConnection::create(io_context_, vote_data_);
        boost::system::error_code ec;
        boost::asio::connect(new_connection->socket(), endpoints, ec);

        if (!ec) {
            boost::asio::read(new_connection->socket(), 
                              boost::asio::buffer(received, num_voters));
            int index = vote_data_.processReceived(received);
            new_connection->sendVKPair(index);
        }
    }

    delete [] received;
}


/* ======================================================================== */


boost::shared_ptr<TCPConnection> TCPConnection::create(
                    boost::asio::io_context& io_context,
                    VoteData& vote_data)
{
    return boost::shared_ptr<TCPConnection>(new TCPConnection(io_context, 
                                                              vote_data));
}


TCPConnection::TCPConnection(boost::asio::io_context& io_context,
                             VoteData& vote_data)
                             :
                             socket_(io_context),
                             vote_data_(vote_data)
{}


void TCPConnection::sendReceived()
{
    boost::asio::async_write(socket_, vote_data_.makeReceivedMsg(),
                             boost::bind(&TCPConnection::handleWriteReceived, 
                                         shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}


void TCPConnection::sendVKPair(int index)
{
    boost::asio::async_write(socket_, vote_data_.makeVKPairMsg(index),
                             boost::bind(&TCPConnection::handleWriteVKPair,
                                         shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}


void TCPConnection::handleWriteReceived(const boost::system::error_code& /*error*/,
                                        size_t /*bytes_transferred*/)
{
    CryptoPP::byte raw_index[4];
    boost::asio::read(socket_, boost::asio::buffer(raw_index),
                      boost::asio::transfer_exactly(4));
    int index = ByteToInt(raw_index);

    if (index >= 0) {
        size_t length = 489 * vote_data_.numOptions();
        auto* in = new CryptoPP::byte[length];
        boost::asio::read(socket_, boost::asio::buffer(in, length), 
                          boost::asio::transfer_exactly(length));  
        vote_data_.processVKPair(in, index);
        delete [] in;
    }
}


void TCPConnection::handleWriteVKPair(const boost::system::error_code& /*error*/,
                                      size_t /*bytes_transferred*/)
{}


/* ======================================================================== */


int ByteToInt(const CryptoPP::byte ch[4])
{
    return static_cast<int>(ch[0]) * 16777216 
         + static_cast<int>(ch[1]) * 65536
         + static_cast<int>(ch[2]) * 256
         + static_cast<int>(ch[3]);
}


void IntToByte(int n, CryptoPP::byte output[4])
{
    int first = n / 16777216;
    output[0] = static_cast<char>(first);

    n %= 16777216;
    int second = n / 65536;
    output[1] = static_cast<char>(second);

    n %= 65536;
    int third = n / 256;
    output[2] = static_cast<char>(third);

    n %= 256;
    output[3] = static_cast<char>(n);
}