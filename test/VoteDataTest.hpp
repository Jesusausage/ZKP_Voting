#ifndef VOTE_DATA_TEST_HPP
#define VOTE_DATA_TEST_HPP


#include "VoteData.hpp"
#include <assert.h>
#include <iostream>


void TestReadOptions();
void TestReadTokens();
void TestReadIDs();
void TestReadIPs();


class VoteDataTest {
public:
    static void testWriteVote();
    static void testWriteKey();
    static void testProcessHashes();
};


#endif