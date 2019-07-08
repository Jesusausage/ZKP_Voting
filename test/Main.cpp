#include "SchnorrProtocolTest.hpp"
#include "OrProtocolTest.hpp"
#include "ECGroupTest.hpp"

//#include <boost/asio.hpp>
#include <ecp.h>


int main() {
    TestNormalSchnorrRun();
    TestSimulatedSchnorrRun();
    std::cout << "SchnorrProtocol test success." << std::endl;
    
    TestNormalOrRun();
    TestSimulatedOrRun();
    std::cout << "OrProtocol test success." << std::endl;

    TestECGroupAddition();
    TestDecodePoint();
    TestPointCompression();
    TestRandomPoint();
    std::cout << "ECGroup test success." << std::endl;

	while (!std::cin.get())
    return 0;
}
