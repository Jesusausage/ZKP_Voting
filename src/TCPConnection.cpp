#include "TCPConnection.hpp"
#include "VoteData.hpp"


using namespace boost::asio::ip;


TCPConnection::TCPConnection(boost::asio::io_context& io_context,
                             VoteData& vote_data)
                             :
                             socket_(io_context),
                             vote_data_(vote_data)
{}


boost::shared_ptr<ServerConnection> ServerConnection::create(boost::asio::io_context& io_context,
                                                             VoteData& vote_data)
{
    return boost::shared_ptr<ServerConnection>(
        new ServerConnection(io_context, vote_data));
}


ServerConnection::ServerConnection(boost::asio::io_context& io_context,
                                   VoteData& vote_data)
                                   :
                                   TCPConnection(io_context, vote_data)
{}


void ServerConnection::makeReceivedMsg()
{
    received_msg_ = vote_data_.received();
    num_voters_ = vote_data_.numVoters();
}


void ServerConnection::sendReceived()
{
    makeReceivedMsg();
    boost::asio::async_write(socket_, boost::asio::buffer(received_msg_, num_voters_),
                             boost::bind(&ServerConnection::handleWriteReceived, 
                                         shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}


void ServerConnection::handleWriteReceived(const boost::system::error_code& /*error*/,
                                           size_t /*bytes_transferred*/)
{
    std::cout << "written received" << std::endl;
    CryptoPP::byte raw_index[4];
    boost::asio::read(socket_, boost::asio::buffer(raw_index));
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


boost::shared_ptr<ClientConnection> ClientConnection::create(boost::asio::io_context& io_context,
                                                             VoteData& vote_data)
{
    return boost::shared_ptr<ClientConnection>(
        new ClientConnection(io_context, vote_data));
}


ClientConnection::ClientConnection(boost::asio::io_context& io_context,
                                   VoteData& vote_data)
                                   :
                                   TCPConnection(io_context, vote_data)
{}


void ClientConnection::makeVKPairMsg(int index)
{
    if (index < 0) {
        vkpair_msg_.resize(4);
        IntToByte(-1, vkpair_msg_.data());
    }
    else {
        int num_options = vote_data_.numOptions();
        vkpair_msg_.resize(489 * num_options + 4);
        
        IntToByte(index, vkpair_msg_.data());
        size_t offset = 4;
        VoteData::readVote(index, vkpair_msg_.data() + offset, num_options);
        offset += 326 * num_options;
        VoteData::readKey(index, vkpair_msg_.data() + offset, num_options);
    }
}


void ClientConnection::sendVKPair(int index)
{
    makeVKPairMsg(index);
    boost::asio::async_write(socket_, boost::asio::buffer(vkpair_msg_),
                             boost::bind(&ClientConnection::handleWriteVKPair,
                                         shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}


void ClientConnection::handleWriteVKPair(const boost::system::error_code& /*error*/,
                                         size_t /*bytes_transferred*/)
{
    std::cout << "written vkpair" << std::endl;
}


int ByteToInt(const CryptoPP::byte ch[4])
{
    int abs = static_cast<int>(ch[1]) * 65536
            + static_cast<int>(ch[2]) * 256
            + static_cast<int>(ch[3]);
    return (ch[0] == 1) ? -abs : abs;
}


void IntToByte(int n, CryptoPP::byte output[4])
{
    output[0] = (n < 0) ? 1 : 0;
    n = abs(n);

    int first = n / 65526;
    output[1] = static_cast<unsigned char>(first);

    n %= 65536;
    int second = n / 256;
    output[2] = static_cast<unsigned char>(second);

    n %= 256;
    output[3] = static_cast<unsigned char>(n);
}