#include <boost/asio.hpp>
#include <iostream>
#include <cryptopp/integer.h>


int main()
{
    using namespace boost::asio;

    io_context io_context;
    ip::tcp::acceptor acceptor(io_context, ip::tcp::endpoint(ip::tcp::v4(), 1300));

    while (true) {
        ip::tcp::socket socket(io_context);
        acceptor.accept(socket);

        int size[1] = {4};
        char msg[128];
        for (int i = 0; i < 128; i++)
            msg[i] = 'A' + (i % 26);

        msg[31] = '\0';
        msg[63] = '\0';
        msg[95] = '\0';
        msg[127] = '\0';

        boost::system::error_code ignored_error;
        boost::asio::write(socket, buffer(size), ignored_error);
        boost::asio::write(socket, buffer(msg), ignored_error);
    }

    return 0;
}