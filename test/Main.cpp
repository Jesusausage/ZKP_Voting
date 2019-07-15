#include "SchnorrProtocolTest.hpp"
#include "ElGamalProtocolTest.hpp"
#include "OrProtocolTest.hpp"
#include "ECGroupTest.hpp"
#include "VoterTest.hpp"
#include "KeyGenTest.hpp"
#include "VerifierTest.hpp"


int main() {
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

    TestECGroupAddition();
    TestDecodePoint();
    TestPointCompression();
    TestRandomPoint();
    std::cout << "ECGroup test success." << std::endl;

    TestVoting();
    std::cout << "Voter test success." << std::endl;

    TestKeyGeneration();
    std::cout << "KeyGen test success." << std::endl;

    TestVerification();
    TestVoteDecryption();
    TestVoteCompression();
    std::cout << "Verification test success." << std::endl;

	while (!std::cin.get())
    return 0;
}
