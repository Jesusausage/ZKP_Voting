#include <boost/asio.hpp>
#include <iostream>


int main()
{
    using namespace boost::asio::ip;

    boost::asio::io_context io;
    tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 1300));

    while (true) {
        tcp::socket socket(io);
        acceptor.accept(socket);      

        std::cout << "running" << std::endl;

        std::string msg_out = "hello from server\r\n\r\n";
        boost::system::error_code ignored_error;
        boost::asio::write(socket, boost::asio::buffer(msg_out), ignored_error);

        {
            std::cout << "reading..." << std::endl;
            boost::asio::streambuf msg_in;

            size_t len = boost::asio::read_until(socket, msg_in, "\r\n\r\n");

            std::cout << "...finished:" << std::endl;
            std::cout << &msg_in << std::endl;
        }
    }

    return 0;
}