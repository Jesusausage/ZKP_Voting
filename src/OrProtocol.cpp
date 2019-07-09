#include "OrProtocol.hpp"


OrProtocol::OrProtocol(std::vector<SigmaProtocol*> sigma_protocols, 
                       int i_known)
                       :
                       _sigma_prots(sigma_protocols), 
                       _i_known(i_known)
{
    _num_prots = sigma_protocols.size();
    assert(_i_known >= 0 && _i_known < _num_prots);
}


void OrProtocol::generateCommitment()
{
    for (int i = 0; i < _num_prots; i++) {
        if (i == _i_known)
            _sigma_prots[i]->generateCommitment();
        else {
            _sigma_prots[i]->generateChallenge();
            _sigma_prots[i]->generateSimulation();
        }
    }
}


void OrProtocol::generateChallenge(CryptoPP::Integer e)
{
    _total_e = 0;
    for (int i = 0; i < _num_prots; i++) {
        if (i != _i_known)
            _total_e += _sigma_prots[i]->challenge();
    }
    assert(e > _total_e);
    _e = e;
}


void OrProtocol::generateResponse()
{
    assert(_e > 0);

    CryptoPP::Integer e_known = _e - _total_e;
    _sigma_prots[_i_known]->generateChallenge(&e_known);
    _sigma_prots[_i_known]->generateResponse();
}


bool OrProtocol::verify()
{
    assert(_e > 0);

    for (SigmaProtocol* prot : _sigma_prots) {
        if (!prot->verify())
            return false;
    }
    if (_e != _total_e + _sigma_prots[_i_known]->challenge())
        return false;

    return true;
}


std::string OrProtocol::getHashData() 
{
    std::string ret;
    for (int i = 0; i < _num_prots; i++)
        ret += _sigma_prots[i]->getHashData();
    return ret;
}


void OrProtocol::generateNIZKP()
{
}