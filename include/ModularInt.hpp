#ifndef MODULARINT_HPP
#define MODULARINT_HPP


#include <iostream>


class ModularInt {
    
public:
    ModularInt() {}
    ModularInt(int modulus);
    ModularInt(int input_val, int modulus);
    ModularInt(const ModularInt& a);
    
    friend ModularInt operator*(ModularInt a, ModularInt b);

    friend bool operator==(ModularInt a, ModularInt b);
    friend bool operator==(ModularInt a, int b);
    friend bool operator==(int a, ModularInt b);

    void operator=(int input_val);

    friend std::ostream& operator<<(std::ostream& o, ModularInt x);

private:
    int _val = 1;
    int _modulus;

    void set_val(int input_val);
};


#endif
