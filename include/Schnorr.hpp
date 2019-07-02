#ifndef SCHNORR_HPP
#define SCHNORR_HPP


#include "SigmaUtils.hpp"


template<typename T>
T GenerateCommitment(Group<T> G, int& r) {
    int range = G.order() - 2;
    srand(time(NULL));
    r = (rand() % range) + 2;
    
    return G.newElement(r);    
}

template<typename T>
bool VerifyResponse(Group<T> G, T public_key, 
		    T commitment, int challenge, int response) {
    T result = G.newElement(response) * G.power(public_key, -challenge);
    return (result == commitment);
}

template<typename T>
T GenerateSimulation(Group<T> G, T public_key,
			int challenge, int response) {
    T response_element = G.newElement(response);
    T challenge_element = G.power(public_key, -challenge);
    T commitment = response_element * challenge_element;

    return commitment;    
}


#endif
