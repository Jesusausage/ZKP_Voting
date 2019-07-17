#include "ECGroupTest.hpp"
#include "SchnorrProtocolTest.hpp"
#include "ElGamalProtocolTest.hpp"
#include "OrProtocolTest.hpp"
#include "VoterTest.hpp"
#include "KeyGenTest.hpp"
#include "VerifierTest.hpp"


int main() {
    TestECGroupAddition();
    TestDecodePoint();
    TestPointCompression();
    TestRandomPoint();
    std::cout << "ECGroup test success." << std::endl;
    
    TestNormalSchnorrRun();
    TestSimulatedSchnorrRun();
    TestSchnorrNIZKP();
    std::cout << "SchnorrProtocol test success." << std::endl;

    TestNormalElGamalRun();
    TestSimulatedElGamalRun();
    TestElGamalNIZKP();
    std::cout << "ElGamalProtocol test success." << std::endl;
    
    TestSchnorrOrRun();
    TestSchnorrOrNIZKP();
    TestElGamalOrRun();
    TestElGamalOrNIZKP();
    std::cout << "OrProtocol test success." << std::endl;

    TestVoting();
    std::cout << "Voter test success." << std::endl;

    TestKeyGeneration();
    std::cout << "KeyGen test success." << std::endl;

    TestVerification();
    TestVoteDecryption();
    // TestVoteCompression();
    std::cout << "Verification test success." << std::endl;

    auto ecg = GenerateECGroup();
    std::cout << ecg.order << std::endl;
    unsigned char enc[32];
    ecg.order.Encode(enc, 32);
    auto o = CryptoPP::Integer(enc, 32);    
    std::cout << o << std::endl;

    return 0;
}
