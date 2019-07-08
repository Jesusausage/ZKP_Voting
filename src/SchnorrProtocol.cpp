#include "SchnorrProtocol.hpp"


SchnorrProtocol::SchnorrProtocol(const EllipticCurve& elliptic_curve,
                                 const CryptoPP::ECPPoint& public_key, 
                                 const CryptoPP::Integer& witness)
                                 :
                                 _curve(&elliptic_curve.curve), 
                                 _base(&elliptic_curve.base),
                                 _order(&elliptic_curve.order),
                                 _pub_key(public_key), 
                                 _w(witness) 
{}


SchnorrProtocol::SchnorrProtocol(const EllipticCurve& elliptic_curve,
                                 const CryptoPP::ECPPoint& public_key)
                                 :
                                 _curve(&elliptic_curve.curve), 
                                 _base(&elliptic_curve.base),
                                 _order(&elliptic_curve.order),
                                 _pub_key(public_key)
{}


void SchnorrProtocol::generateCommitment()
{
    _u = RandomInteger(2, *_order);    
    _commitment = _curve->Multiply(_u, *_base);
}


void SchnorrProtocol::generateChallenge(CryptoPP::Integer* e /*= nullptr*/)
{
    if (e) {
        _e = *e;
    }
    else {
        _e = RandomInteger(1, *_order);
    }
}


bool SchnorrProtocol::generateResponse()
{
    if (_e.IsZero())
        return false;

    auto we = a_times_b_mod_c(_w, _e, *_order);
    _s = (_u + we) % *_order;
    return true;
}


bool SchnorrProtocol::verify()
{
    auto alpha = _curve->Multiply(_s, *_base);
    auto beta = _curve->Multiply(_e, _curve->Inverse(_pub_key));

    auto result = _curve->Add(alpha, beta);
    return (_commitment == result);
}


bool SchnorrProtocol::generateSimulation()
{
    if (!_e)
        return false;

    _s = RandomInteger(1, *_order);

    auto alpha = _curve->Multiply(_s, *_base);
    auto beta = _curve->Multiply(_e, _curve->Inverse(_pub_key));
    _commitment = _curve->Add(alpha, beta);
    return true;
}