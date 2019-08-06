#include "ECGroupTest.hpp"
#include "SchnorrProtocolTest.hpp"
#include "ElGamalProtocolTest.hpp"
#include "OrProtocolTest.hpp"
#include "VoterTest.hpp"
#include "KeyGenTest.hpp"
#include "VerifierTest.hpp"
#include "VoteDataTest.hpp"
#include "TCPServerTest.hpp"

#include <boost/thread/thread.hpp>


int main() {
    // TestECGroupAddition();
    // TestDecodePoint();
    // TestPointCompression();
    // TestRandomPoint();
    // TestWrite();
    // std::cout << "ECGroup test success." << std::endl;
    
    // TestNormalSchnorrRun();
    // TestSimulatedSchnorrRun();
    // TestSchnorrNIZKP();
    // std::cout << "SchnorrProtocol test success." << std::endl;

    // TestNormalElGamalRun();
    // TestSimulatedElGamalRun();
    // TestElGamalNIZKP();
    // std::cout << "ElGamalProtocol test success." << std::endl;
    
    // TestSchnorrOrRun();
    // TestSchnorrOrNIZKP();
    // TestElGamalOrRun();
    // TestElGamalOrNIZKP();
    // std::cout << "OrProtocol test success." << std::endl;

    // TestVoting();
    // std::cout << "Voter test success." << std::endl;

    // TestKeyGeneration();
    // std::cout << "KeyGen test success." << std::endl;

    // TestVerification();
    // TestVoteDecryption();
    // std::cout << "Verification test success." << std::endl; 

    // VoteDataTest::testReadOptionsAndIPs();
    // VoteDataTest::testReadTokensAndIDs();
    // VoteDataTest::testWriteVote();
    // VoteDataTest::testWriteKey();
    // VoteDataTest::testProcessHashes();
    // VoteDataTest::testSuccessfulVote();
    // VoteDataTest::testGetUserVote();
    // std::cout << "VoteData test success." << std::endl;

    TestIntByteConversion();
    std::cout << "TCPServer test success." << std::endl;

    auto ecg = GenerateECGroup();
    auto gen = GenerateECBase();
    VoteData data(ecg, gen, 10, 5);
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
