#ifndef ELGAMAL_HPP
#define ELGAMAL_HPP


#include "SigmaUtils.hpp"
#include <utility>


template<typename T>
std::pair<T, T> GenerateCommitment(Group<T> G, int& u, T g ,T h) {
    int range = G.order() - 2;
    srand(time(NULL));
    u = (rand() % range) + 2;

    T r1 = G.power(g, u);
    T r2 = G.power(h, u);

    return std::pair<T, T>(r1, r2);
}


int GenerateResponse(int u, int challenge, int witness);


template<typename T>
bool VerifyResponse(Group<T> G, std::pair<T, T> public_key, T g, T h, 
                    std::pair<T, T> commitment, int challenge, int response) {
    T r1 = G.power(g, response) * G.power(public_key.first, -challenge);
    T r2 = G.power(h, response) * G.power(public_key.second, -challenge);

    return (r1 == commitment.first) && (r2 == commitment.second);
}


template<typename T>
std::pair<T, T> GenerateSimulation(Group<T> G, std::pair<T, T> public_key,
				   T g, T h, int challenge, int response) {
    T r1 = G.power(g, response) * G.power(public_key.first, -challenge);
    T r2 = G.power(h, response) * G.power(public_key.second, -challenge);

    std::pair<T, T> commitment(r1, r2);
    return commitment;
}


#endif
