#include "TCPConnection.hpp"
#include "VoteData.hpp"


using namespace boost::asio::ip;


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
    message_ = vote_data_.makeReceivedMsg();
    boost::asio::async_write(socket_, message_,
                             boost::bind(&TCPConnection::handleWriteReceived, 
                                         shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}


void TCPConnection::sendVKPair(int index)
{
    message_ = vote_data_.makeVKPairMsg(index);
    boost::asio::async_write(socket_, message_,
                             boost::bind(&TCPConnection::handleWriteVKPair,
                                         shared_from_this(),
                                         boost::asio::placeholders::error,
                                         boost::asio::placeholders::bytes_transferred));
}


void TCPConnection::handleWriteReceived(const boost::system::error_code& /*error*/,
                                        size_t /*bytes_transferred*/)
{
    std::cout << "written received" << std::endl;
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
{
    std::cout << "written vkpair" << std::endl;
}


/* ======================================================================== */


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