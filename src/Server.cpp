#include <boost/asio.hpp>
#include <iostream>
#include <cryptopp/integer.h>
#include "KeyGen.hpp"


int main()
{
    using namespace boost::asio;

    io_context io_context;
    ip::tcp::acceptor acceptor(io_context, ip::tcp::endpoint(ip::tcp::v4(), 1300));

    while (true) {
        ip::tcp::socket socket(io_context);
        acceptor.accept(socket);

        auto ecg = GenerateECGroup();
        auto gen = ecg.base;
        auto id_key = CryptoPP::Integer(27);
        auto id = ecg.curve.Multiply(id_key, gen);
        std::vector<CryptoPP::ECPPoint> token_sums;
        for (int i = 0; i < 10; i++) {
            token_sums.push_back(ecg.curve.Multiply(42 + i, gen));
        }

        KeyGen key_gen(ecg, gen, token_sums, id);
        key_gen.setIDKey(id_key);
        Key key = key_gen.getKeysAndProofs();
        CryptoPP::byte msg[1630];
        int opts[1];
        key.serialise(msg, opts[0]);        

        std::cout << "running" << std::endl;

        boost::system::error_code ignored_error;
        boost::asio::write(socket, buffer(opts), ignored_error);
        boost::asio::write(socket, buffer(msg), ignored_error);
    }

    return 0;
}