#include "SchnorrProtocol.hpp"


SchnorrProtocol::SchnorrProtocol(Group<ModularInt> group, 
                                 ModularInt generator, 
                                 ModularInt public_key, 
                                 int witness)
                                 : 
                                 SigmaProtocol(group), 
                                 _g(generator), 
                                 _pub_key(public_key), 
                                 _w(witness) 
{
	srand(static_cast<long int>(time(nullptr)));
}


SchnorrProtocol::SchnorrProtocol(Group<ModularInt> group, 
                                 ModularInt generator, 
                                 ModularInt public_key)
                                 :
                                 SigmaProtocol(group),
                                 _g(generator),
                                 _pub_key(public_key)
{
	srand(static_cast<long int>(time(nullptr)));
}


void SchnorrProtocol::generateCommitment()
{
    int range = (G.order() / 2) - 2;
    _u = (rand() % range) + 2;
    
    _commitment = G.power(_g, _u);
}


void SchnorrProtocol::generateChallenge(int e /*= 0*/)
{
    if (e) {
        _e = e;
    }
    else {
        _e = (rand() % (G.order() - 1)) + 1;
    }
}


bool SchnorrProtocol::generateResponse()
{
    if (!_e)
        return false;

    _s = (_u + (_w * _e)) % G.order();
    return true;
}


bool SchnorrProtocol::verify()
{
    ModularInt result = G.power(_g, _s) * G.power(_pub_key, -_e);
    return (result == _commitment);
}


bool SchnorrProtocol::generateSimulation()
{
    if (!_e)
        return false;

    _s = rand() % G.order();
    _commitment = G.power(_g, _s) * G.power(_pub_key, -_e);
    return true;
}