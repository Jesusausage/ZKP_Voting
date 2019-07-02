#ifndef GROUP_HPP
#define GROUP_HPP


#include "ModularInt.hpp"


template<typename T>
class Group {

public:
    Group(T generator, int modulus) {
	_generator = generator;
	_order = modulus - 1;
	_identity = positivePower(generator, _order);
    }

    T newElement(int exp = 1) {
	return power(_generator, exp);
    }
    
    T power(T a, int exp) {
	if (exp > 0) {
	    return positivePower(a, exp);
	} else {
	    return power(a, _order + exp);
	}
    }

    T inverse(T a) {
	return power(a, _order - 1);
    }

    T identity() {
	return _identity;
    }

    T generator() {
	return _generator;
    }

    int order() {
	return _order;
    }
    
private:
    T _generator;
    int _order;
    T _identity;

    T positivePower(T a, int exp) {
	if (exp == 1) {
	    return a;
	} else if (exp % 2 == 0) {
	    return power(a * a, exp/2);
	} else {
	    return a * power(a * a, (exp - 1)/2);
	}
    }
};


#endif
