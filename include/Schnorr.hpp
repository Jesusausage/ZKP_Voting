#ifndef SCHNORR_H
#define SCHNORR_H


#include "Group.hpp"
#include <stdlib.h>
#include <time.h>


long long int GetPrime();

bool TestPrime(long long int p);

template<typename T>
Group<T> GetGroup() {
    long long int p = GetPrime();
    const long long int order = p;
    Group<ModularInt<order>> G(order);

    return G;
}

#endif
