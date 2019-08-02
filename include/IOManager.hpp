#ifndef IO_MANAGER_HPP
#define IO_MANAGER_HPP


#include "VoteData.hpp"
#include <boost/bind.hpp>
#include <boost/asio.hpp>


class IOManager {
public:

private:
    boost::asio::io_context io_context_;
    
};


#endif