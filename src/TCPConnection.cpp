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