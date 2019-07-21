#include <boost/asio.hpp>
#include <iostream>
#include <cryptopp/integer.h>
#include "KeyGen.hpp"
#include "Verifier.hpp"


int main()
{
    boost::asio::io_context io_context;

    boost::asio::ip::tcp::resolver resolver(io_context);

    boost::asio::ip::tcp::resolver::results_type endpoints =
        resolver.resolve("192.168.1.29", "1300");

    boost::asio::ip::tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);

    int size[1];
    boost::system::error_code error;
    boost::asio::read(socket, boost::asio::buffer(size), 
                      boost::asio::transfer_exactly(4));
    
    assert(size[0] == 10);

    CryptoPP::byte msg[1630];
    boost::asio::read(socket, boost::asio::buffer(msg));

    auto ecg = GenerateECGroup();
    auto gen = ecg.base;
    auto id_key = CryptoPP::Integer(27);
    auto id = ecg.curve.Multiply(id_key, gen);
    std::vector<CryptoPP::ECPPoint> token_sums;
    for (int i = 0; i < 10; i++) {
        token_sums.push_back(ecg.curve.Multiply(42 + i, gen));
    }

    Key ey(msg, 10, ecg.curve);
    for (int i = 0; i < 10; i++) {
        std::cout << i << std::endl;
        ElGamalProtocol prot(ecg, gen, 0);
        prot.setParams(token_sums[i], id, ey.value(i));
        assert(prot.verifyNIZKP(ey.proof(i)) == true);
    }

    return 0;
}