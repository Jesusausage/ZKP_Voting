#include "OrProtocol.hpp"


OrProtocol::OrProtocol(SigmaProtocol* sigma0, 
                       SigmaProtocol* sigma1,
                       bool know0)
                       :
                       _sigma0(sigma0),
                       _sigma1(sigma1),
                       _know0(know0)
{
    int s0 = _sigma0->challengeSize();
    int s1 = _sigma1->challengeSize();
    _e_size = (s0 > s1) ? s0 : s1;
	srand(static_cast<long int>(time(nullptr)));
}


void OrProtocol::generateCommitment()
{
    if (_know0) {
        _sigma0->generateCommitment();

        _sigma1->generateChallenge();
        _sigma1->generateSimulation();
    }
    else {
        _sigma1->generateCommitment();

        _sigma0->generateChallenge();
        _sigma0->generateSimulation();
    }
}


void OrProtocol::generateChallenge(int* e /*= nullptr*/)
{
    if (e) {
        _e = *e;
    }
    else {
        _e = (rand() % _e_size) + _e_size;
    }
}


bool OrProtocol::generateResponse()
{
    if (!_e)
        return false;

    if (_know0) {
        int e0 = _e - _sigma1->challenge();
        _sigma0->generateChallenge(&e0);
        _sigma0->generateResponse();
    }
    else {
        int e1 = _e - _sigma0->challenge();
        _sigma1->generateChallenge(&e1);
        _sigma1->generateResponse();
    }
    return true;
}


bool OrProtocol::verify()
{
    if (_e != _sigma0->challenge() + _sigma1->challenge())
        return false;
    if (!_sigma0->verify())
        return false;
    if (!_sigma1->verify())
        return false;

    return true;
}


bool OrProtocol::generateSimulation()
{
    if (!_e)
        return false;

    _sigma0->generateChallenge();
    int e1 = _e - _sigma0->challenge();
    _sigma1->generateChallenge(&e1);

    _sigma0->generateSimulation();
    _sigma1->generateSimulation();
    return true;
}