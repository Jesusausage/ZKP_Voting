#include "TCPServer.hpp"
#include "TCPClient.hpp"
#include <boost/thread/thread.hpp>
#include "ParamGen.hpp"


int main() {
    // generateParams();
    std::cout << "Generated params." << std::endl;

    auto ecg = GenerateECGroup();
    auto gen = GenerateECBase();    
    PublicData pub(ecg, 10, 5);
    PrivateData priv(5);
    VoteData data(ecg, gen, pub, priv);

    boost::asio::io_context sio;
    boost::asio::io_context cio;
    TCPServer ser(data, sio);
    TCPClient cli(data, cio);
    boost::thread t1(boost::bind(&boost::asio::io_context::run, &sio));
    boost::thread t2(boost::bind(&boost::asio::io_context::run, &cio));
    t1.join();
    t2.join();

    return 0;
}
