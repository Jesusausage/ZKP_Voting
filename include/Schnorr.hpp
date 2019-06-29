#ifndef SCHNORR_H
#define SCHNORR_H


#include "Group.hpp"
#include <stdlib.h>
#include <time.h>
#include <math.h>


long long int GetPrime();

bool TestPrime(long long int p);

template<typename T>
Group<T> GetGroup() {
    long long int p = GetPrime();
    ModularInt e(1, p);
    Group<ModularInt> G(p, e);

    return G;
}

#endif
