#include <boost/asio.hpp>
#include <iostream>
#include <cryptopp/integer.h>


int main()
{
    using namespace boost::asio;

    io_context io_context;
    ip::tcp::acceptor acceptor(io_context, 
                            ip::tcp::endpoint(ip::tcp::v4(), 1300));

    while (true) {
        ip::tcp::socket socket(io_context);
        acceptor.accept(socket);

        CryptoPP::Integer potatoes[24];
        for (int i = 0; i < 24; i++) {
            potatoes[i] = i;
        }

        boost::system::error_code ignored_error;
        boost::asio::write(socket, buffer(potatoes), ignored_error);
    }

    return 0;
}