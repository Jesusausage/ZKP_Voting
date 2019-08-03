#ifndef TCP_SERVER_HPP
#define TCP_SERVER_HPP


#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>


class TCPConnection;
class TCPClient;
class VoteData;


class TCPServer {
public:
    TCPServer(const VoteData& vote_data, 
              TCPClient* client,
              boost::asio::io_context& io_context);
              
    boost::asio::const_buffer makeHashesMessage();
    boost::asio::const_buffer makeVKMessage(int index);

    int waitForClientRequest(boost::asio::ip::tcp::socket& sock);

private:
    const VoteData& vote_data_;
    TCPClient* client_;
    boost::asio::io_context& io_context_;
    boost::asio::ip::tcp::acceptor acceptor_;

    void startAccept();
    void handleAccept(boost::shared_ptr<TCPConnection> new_connection,
                      const boost::system::error_code& error);
};


/* ====================================================================== */


class TCPConnection : public boost::enable_shared_from_this<TCPConnection> {
public:
    static boost::shared_ptr<TCPConnection> create(
                            boost::asio::io_context& io_context,
                            TCPServer* server);
    void start();
    boost::asio::ip::tcp::socket& socket();

private:
    boost::asio::ip::tcp::socket socket_;
    TCPServer* server_;

    TCPConnection(boost::asio::io_context& io_context, TCPServer* server);

    void handleWrite(const boost::system::error_code& /*error*/,
                     size_t /*bytes_transferred*/);
};


#endif