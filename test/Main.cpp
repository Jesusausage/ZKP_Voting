#include "ECGroupTest.hpp"
#include "SchnorrProtocolTest.hpp"
#include "ElGamalProtocolTest.hpp"
#include "OrProtocolTest.hpp"
#include "VoterTest.hpp"
#include "KeyGenTest.hpp"
#include "VerifierTest.hpp"
#include "VoteDataTest.hpp"


int main() {
    TestECGroupAddition();
    TestDecodePoint();
    TestPointCompression();
    TestRandomPoint();
    TestWrite();
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
    std::cout << "Verification test success." << std::endl; 

    TestReadOptions();
    TestReadTokens();
    TestReadIDs();
    TestReadIPs();
    TestWriteVote();
    TestWriteKey();
    std::cout << "VoteData test success." << std::endl;

    return 0;
}
