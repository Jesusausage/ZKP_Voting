#ifndef IP_SERVER_HPP
#define IP_SERVER_HPP


#include "ECGroup.hpp"
#include <boost/asio.hpp>
#include <vector>


class IPServer {
public:
    void run();

private:
    std::vector<std::string> ips_;
};


#endif