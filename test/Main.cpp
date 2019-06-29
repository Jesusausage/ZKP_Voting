#include "GroupTest.hpp"
#include "ModularIntTest.hpp"


int main() {
    TestModularIntConstructor();
    TestMultiply();
    TestAssignment();
    std::cout << "ModularInt test success.\n";
    
    TestGroupConstructor();
    TestPower();
    TestInverse();
    std::cout << "Group test success.\n";
    
    return 0;
}
