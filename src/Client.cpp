#include <boost/asio.hpp>
#include <iostream>
#include <cryptopp/integer.h>


int main()
{
    boost::asio::io_context io_context;

    boost::asio::ip::tcp::resolver resolver(io_context);

    boost::asio::ip::tcp::resolver::results_type endpoints =
        resolver.resolve("146.169.218.129", "1300");

    boost::asio::ip::tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);

    size_t size[1];
    boost::system::error_code error;
    boost::asio::read(socket, boost::asio::buffer(size), 
                      boost::asio::transfer_exactly(4));

    for (int i = 0; ; i++) {
        char msg[32];
        boost::system::error_code error;

        boost::asio::read(socket, boost::asio::buffer(msg), error);
        if (error == boost::asio::error::eof)
            break;

        std::cout << msg << std::endl;
    }

    return 0;
}