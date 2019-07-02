#include "SigmaUtils.hpp"


int GetPrime() {
    srand(static_cast<long int>(time(nullptr)));
    int p = (rand() % 900 + 100) * 10 + 1;
    while (!TestPrime(p) || !TestPrime((p - 1) / 2)) {
	p += 2;
    }

    return p;
}


bool TestPrime(int p) {
    if (p % 2 == 0)
	return false;
    
    double ceil = sqrt(p);
    
    int divisor = 3;
    while (divisor <= ceil) {
	if (p % divisor == 0)
	    return false;
	divisor += 2;
    }
    
    return true;
}


int GenerateChallenge(int order) {
	srand(static_cast<long int>(time(nullptr)));
    return (rand() % (order - 1)) + 1;
}


int GenerateResponse(int u, int challenge, int witness) {
    return u + witness*challenge;
}
