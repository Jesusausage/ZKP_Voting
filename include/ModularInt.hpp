#ifndef MODULARINT_H
#define MODULARINT_H


#include <iostream>


class ModularInt {
    
public:
    ModularInt() {}
    
    ModularInt(int input_val, unsigned int order) {
        set_val(input_val);
	_order = order;
    }
    
    friend ModularInt operator*(ModularInt a, ModularInt b) {
	return (a._val * b._val) % _order;
    }

    friend bool operator==(ModularInt a, ModularInt b) {
	if (a._order != b._order)
	    return false;
	return (a._val % order == b._val % order);
    }

    friend bool operator==(ModularInt a, int b) {
	return (a._val % order == b % order);
    }

    friend bool operator==(int a, ModularInt b) {
	return (a % order == b._val % order);
    }

    void operator=(ModularInt a) {
	
        _val = a._val;
    }

    void operator=(int input_val) {
        set_val(input_val);
    }

    friend std::ostream& operator<<(std::ostream& o, ModularInt x) {
	return o << x._val;
    }

private:
    int _val = 1;
    unsigned int _order;

    void set_val(int input_val) {
	if (input_val >= 0) {
	    _val = input_val % ORDER;
	} else {
	    _val = (input_val % ORDER) + ORDER;
	}
    }
};


#endif
