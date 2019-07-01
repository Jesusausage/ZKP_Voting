#include "ModularInt.hpp"


ModularInt::ModularInt(int modulus) {
    _modulus = modulus;
}


ModularInt::ModularInt(int input_val, int modulus) {
    _modulus = modulus;
    set_val(input_val);
}


ModularInt::ModularInt(const ModularInt& a) {
    _modulus = a._modulus;
    _val = a._val;
}


ModularInt operator*(ModularInt a, ModularInt b) {
    int modulus = a._modulus;
    int val = (a._val * b._val) % modulus;
    return ModularInt(val, modulus);
}


bool operator==(ModularInt a, ModularInt b) {
    if (a._modulus != b._modulus)
	return false;
    int modulus = a._modulus;
    return (a._val % modulus == b._val % modulus);
}


bool operator==(ModularInt a, int b) {
    int modulus = a._modulus;
    return (a._val % modulus == b % modulus);
}


bool operator==(int a, ModularInt b) {
    int modulus = b._modulus;
    return (a % modulus == b._val % modulus);
}


void ModularInt::operator=(int input_val) {
    set_val(input_val);
}


std::ostream& operator<<(std::ostream& o, ModularInt x) {
    return o << x._val << " mod" << x._modulus;
}


void ModularInt::set_val(int input_val) {
    if (input_val >= 0) {
	_val = input_val % _modulus;
    } else {
	_val = (input_val % _modulus) + _modulus;
    }
}
