#include "GroupTest.hpp"
#include "ModularIntTest.hpp"
#include "SigmaUtilsTest.hpp"
#include "SchnorrTest.hpp"
#include "ElgamalTest.hpp"
#include "SchnorrProtocolTest.hpp"
#include "OrProtocolTest.hpp"
#include "ECGroupTest.hpp"

//#include <boost/asio.hpp>
#include <ecp.h>


int main() {
    TestModularIntConstructor();
    TestMultiply();
    TestAssignment();
    std::cout << "ModularInt test success." << std::endl;
    
    TestGroupConstructor();
    TestPower();
    TestInverse();
    std::cout << "Group test success." << std::endl;

    TestGetPrime();
    TestGetGroup();
    std::cout << "SigmaUtils test success." << std::endl;
    
    TestGenerateCommitment();
    TestVerification();
    TestSimulation();
    std::cout << "Schnorr test success." << std::endl;

    TestGenerateElgamalCommitment();
    TestElgamalVerification();
    TestElgamalSimulation();
    std::cout << "Elgamal test success." << std::endl;

    TestNormalSchnorrRun();
    TestSimulatedSchnorrRun();
    std::cout << "SchnorrProtocol test success." << std::endl;
    
    TestNormalOrRun();
    TestSimulatedOrRun();
    std::cout << "OrProtocol test success." << std::endl;

    TestECGroupAddition();
    TestDecodePoint();
    TestPointCompression();
    std::cout << "ECGroup test success." << std::endl;

	while (!std::cin.get())
    return 0;
}
