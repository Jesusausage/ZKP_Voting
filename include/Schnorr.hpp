#ifndef SCHNORR_H
#define SCHNORR_H


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

template<typename T>
T GenerateCommitment(Group<T> G, int& r) {
    int range = G.order() - 2;
    srand(time(NULL));
    r = (rand() % range) + 2;
    
    return G.newElement(r);    
}

int GenerateChallenge(int order);

int GenerateResponse(int r, int challenge, int witness);

template<typename T>
bool VerifyResponse(Group<T> G, T public_key, 
		    T commitment, int challenge, int response) {
    T result = G.newElement(response) * G.power(public_key, -challenge);
    return (result == commitment);
}

#endif
