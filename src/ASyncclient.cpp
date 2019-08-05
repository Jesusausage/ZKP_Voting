#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    boost::asio::io_context io_context;

    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints =
      resolver.resolve(argv[1], "1337");

    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);

    for (;;)
    {
      boost::array<char, 128> buf;
      boost::system::error_code error;

      boost::asio::read(socket, boost::asio::buffer(buf), 
                        boost::asio::transfer_exactly(4), error);

      if (error == boost::asio::error::eof)
        break; // Connection closed cleanly by peer.
      else if (error)
        throw boost::system::system_error(error); // Some other error.

      std::cout.write(buf.data(), 4);
      std::cout.flush();
      for (int k = 0; k < 10; k++) {
        int x = 69696969;
        for (int i = 0; i < 9999999; i++) {
            int y = x % 97;
            x = y + 69696969;
        }
      }
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}