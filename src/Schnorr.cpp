#include "Schnorr.hpp"


long long int GetPrime() {
    srand(time(NULL));
    long long int p = (rand() % 9000000 + 1000000) * 10 + 1;
    while (!TestPrime(p)) {
	n += 2;
    }
    
    return p;
}


bool TestPrime(long long int p) {
    double ceil = sqrt(p);
    int divisor = 3;
    while (divisor <= ceil) {
	if (n % divisor == 0) return false;
	divisor += 2;
    }
    return true;
}
