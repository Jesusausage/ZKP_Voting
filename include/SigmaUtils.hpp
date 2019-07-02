#ifndef SIGMAUTILS_HPP
#define SIGMAUTILS_HPP


#include "Group.hpp"
#include <stdlib.h>
#include <time.h>
#include <math.h>


int GetPrime();

bool TestPrime(int p);

template<typename T>
Group<T> GetGroup() {
    int p = GetPrime();

    srand(time(NULL));
    int g_val = (rand() % (p - 2)) + 2;
    ModularInt g(g_val, p);
    
    return Group<ModularInt>(g, p);
}

int GenerateChallenge(int order);

int GenerateResponse(int r, int challenge, int witness);

#endif
