#ifndef VOTE_DATA_TEST_HPP
#define VOTE_DATA_TEST_HPP


#include "VoteData.hpp"
#include <assert.h>
#include <iostream>





class VoteDataTest {
public:
    static void testReadOptions();
    static void testReadTokens();
    static void testReadIDs();
    static void testReadIPs();
    static void testWriteVote();
    static void testWriteKey();
    static void testProcessHashes();
};


#endif