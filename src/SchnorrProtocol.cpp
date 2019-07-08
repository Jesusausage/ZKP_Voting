#include "SchnorrProtocol.hpp"


SchnorrProtocol::SchnorrProtocol(CryptoPP::ECP curve, 
                                 CryptoPP::ECPPoint base, 
                                 CryptoPP::Integer order, 
                                 CryptoPP::ECPPoint public_key, 
                                 CryptoPP::Integer witness)
                                 :
                                 _curve(curve), 
                                 _base(base), 
                                 _order(order), 
                                 _pub_key(public_key), 
                                 _w(witness) 
{}


SchnorrProtocol::SchnorrProtocol(CryptoPP::ECP curve, 
                                 CryptoPP::ECPPoint base, 
                                 CryptoPP::Integer order, 
                                 CryptoPP::ECPPoint public_key)
                                 :
                                 _curve(curve), 
                                 _base(base), 
                                 _order(order), 
                                 _pub_key(public_key)
{}


void SchnorrProtocol::generateCommitment()
{
    _u = RandomCoeff(_curve);    
    _commitment = _curve.Multiply(_u, _base);
}


void SchnorrProtocol::generateChallenge(CryptoPP::Integer* e /*= nullptr*/)
{
    if (e) {
        _e = *e;
    }
    else {
        _e = RandomCoeff(_curve);
    }
}


bool SchnorrProtocol::generateResponse()
{
    if (_e.IsZero())
        return false;

    auto we = a_times_b_mod_c(_w, _e, _order);
    _s = (_u + we) % _order;
    return true;
}


bool SchnorrProtocol::verify()
{
    auto alpha = _curve.Multiply(_s, _base);
    auto beta = _curve.Multiply(_e, _curve.Inverse(_pub_key));

    auto result = _curve.Add(alpha, beta);
    return (_commitment == result);
}


bool SchnorrProtocol::generateSimulation()
{
    if (!_e)
        return false;

    _s = RandomCoeff(_curve);

    auto alpha = _curve.Multiply(_s, _base);
    auto beta = _curve.Multiply(_e, _curve.Inverse(_pub_key));
    _commitment = _curve.Add(alpha, beta);
    return true;
}