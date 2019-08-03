#include <boost/asio.hpp>
#include <iostream>


int main(int argc, char** argv)
{
    using namespace boost::asio::ip;

    if (argc != 2)
        return 1;

    boost::asio::io_context io_context;
    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve(argv[1], "1300");

    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);

    {
        std::cout << "reading..." << std::endl;
        boost::asio::streambuf msg_in;

        size_t len = boost::asio::read_until(socket, msg_in, "\r\n\r\n");

        std::cout << "...finished:" << std::endl;
        std::cout << &msg_in << std::endl;
    }

    std::string msg_out = "hello from client\r\n\r\n";
    boost::system::error_code ignored_error;
    boost::asio::write(socket, boost::asio::buffer(msg_out), ignored_error);

    return 0;
}