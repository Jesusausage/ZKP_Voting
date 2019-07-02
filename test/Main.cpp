#include "GroupTest.hpp"
#include "ModularIntTest.hpp"
#include "SchnorrTest.hpp"
#include "ElgamalTest.hpp"


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
    TestGenerateCommitment();
    TestVerification();
    TestSimulation();
    std::cout << "Schnorr test success.\n";

    TestGenerateElgamalCommitment();
    TestElgamalVerification();
    TestElgamalSimulation();
    std::cout << "Elgamal test success.\n";
    
    return 0;
}
