#include "TCPClient.hpp"
#include "VoteData.hpp"


using namespace boost::asio::ip;


TCPClient::TCPClient(const VoteData& vote_data, 
                     boost::asio::io_context& io_context)
                     :
                     vote_data_(vote_data),
                     io_context_(io_context)
{}


int TCPClient::receiveRequest(tcp::socket& sock)
{
    int req[1];
    boost::system::error_code error;

    auto end = std::chrono::system_clock::now() + std::chrono::seconds(5);

    while (std::chrono::system_clock::now() < end) {
        boost::asio::read(sock, boost::asio::buffer(req), error);
        if (error == boost::asio::error::eof)
            break;
    }

    return req[0];
}