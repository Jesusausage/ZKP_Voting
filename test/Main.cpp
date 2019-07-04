#include "GroupTest.hpp"
#include "ModularIntTest.hpp"
#include "SigmaUtilsTest.hpp"
#include "SchnorrTest.hpp"
#include "ElgamalTest.hpp"
#include "SchnorrProtocolTest.hpp"
#include "OrProtocolTest.hpp"


int main() {
    TestModularIntConstructor();
    TestMultiply();
    TestAssignment();
    std::cout << "ModularInt test success.\n";
    
    TestGroupConstructor();
    TestPower();
    TestInverse();
    std::cout << "Group test success.\n";

    TestGetPrime();
    TestGetGroup();
    std::cout << "SigmaUtils test success.\n";
    
    TestGenerateCommitment();
    TestVerification();
    TestSimulation();
    std::cout << "Schnorr test success.\n";

    TestGenerateElgamalCommitment();
    TestElgamalVerification();
    TestElgamalSimulation();
    std::cout << "Elgamal test success.\n";

    TestNormalSchnorrRun();
    TestSimulatedSchnorrRun();
    std::cout << "SchnorrProtocol test success.\n";
    
    TestNormalOrRun();
    TestSimulatedOrRun();
    std::cout << "OrProtocol test success.\n";

	while (!std::cin.get())
    return 0;
}
