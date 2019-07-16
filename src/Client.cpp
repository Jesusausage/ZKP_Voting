#include <boost/asio.hpp>
#include <iostream>
#include <cryptopp/integer.h>


int main()
{
    boost::asio::io_context io_context;

    boost::asio::ip::tcp::resolver resolver(io_context);

    boost::asio::ip::tcp::resolver::results_type endpoints =
        resolver.resolve("146.169.221.213", "1300");

    boost::asio::ip::tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);

    while(true) {
        CryptoPP::Integer potatoes[24];
        boost::system::error_code error;

        size_t len = socket.read_some(boost::asio::buffer(potatoes), error);

        if (error == boost::asio::error::eof)
            break;
        else if (error)
            std::cerr << "everything broken\nwell done" << std::endl;

        std::cout << potatoes[4] << std::endl;
    }

    return 0;
}