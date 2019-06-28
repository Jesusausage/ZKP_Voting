#ifndef GROUP_H
#define GROUP_H


#include "ModularInt.hpp"


template<typename T>
class Group {

public:
    Group(int order) : _order(order) {
	T element;
	_identity = power(element, order - 1);
    }
    
    T power(T a, int exp) {
	return (exp == 0) ? _identity :
	    (exp == 1) ? a :
	    (exp % 2 == 0) ? power(a * a, exp/2) :
	    a * power(a * a, (exp - 1)/2);
    }

    T inverse(T a) {
	return power(a, _order - 2);
    }

    T identity() { return _identity; }
    
private:
    T _identity;
    int _order;
};



#endif
