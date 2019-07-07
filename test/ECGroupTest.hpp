#ifndef EC_GROUP_TEST_HPP
#define EC_GROUP_TEST_HPP


#include "ECGroup.hpp"
#include <assert.h>
#include <iostream>


void TestECGroupAddition();
void TestDecodePoint();
void TestPointCompression();
void TestRandomPoint();


#endif