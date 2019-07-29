#ifndef VOTE_DATA_HPP
#define VOTE_DATA_HPP


#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>


class TCPConnection : public boost::enable_shared_from_this<TCPConnection> {
public:
    static boost::shared_ptr<TCPConnection> create(boost::asio::io_context& io_context);
    void start();
    boost::asio::ip::tcp::socket& socket();

private:
    boost::asio::ip::tcp::socket socket_;
    std::string some_stuff_;

    TCPConnection(boost::asio::io_context& io_context);

    void handleWrite(const boost::system::error_code&, size_t);
};


class TCPServer {
public:
    TCPServer(boost::asio::io_context& io_context);

private:
    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;

    void startAccept();
    void handleAccept(boost::shared_ptr<TCPConnection> new_connection,
                      const boost::system::error_code& error);
};


#endif