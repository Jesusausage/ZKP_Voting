#ifndef MODULARINT_H
#define MODULARINT_H


#include <iostream>


class ModularInt {
    
public:
    ModularInt(int order) : _order(order) {}
    
    ModularInt(int input_val, int order) : _order(order) {	
	if (input_val >= 0) {
	    _val = input_val % order;
	} else {
	    _val = (input_val % order) + order;
	}
    }

    ModularInt(const ModularInt& a) {
	_order = a._order;
	_val = a._val;
    }
    
    friend ModularInt operator*(ModularInt a, ModularInt b) {
	int order = a._order;
	int val = (a._val * b._val) % order;
	return ModularInt(val, order);
    }

    friend bool operator==(ModularInt a, ModularInt b) {
	if (a._order != b._order)
	    return false;
	int order = a._order;
	return (a._val % order == b._val % order);
    }

    friend bool operator==(ModularInt a, int b) {
	int order = a._order;
	return (a._val % order == b % order);
    }

    friend bool operator==(int a, ModularInt b) {
	int order = b._order;
	return (a % order == b._val % order);
    }

    void operator=(int input_val) {
        set_val(input_val);
    }

    friend std::ostream& operator<<(std::ostream& o, ModularInt x) {
	return o << x._val << " mod" << x._order;
    }

private:
    int _val = 1;
    int _order;

    void set_val(int input_val) {
	if (input_val >= 0) {
	    _val = input_val % _order;
	} else {
	    _val = (input_val % _order) + _order;
	}
    }
};


#endif
