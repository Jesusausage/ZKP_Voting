#ifndef VOTE_DATA_TEST_HPP
#define VOTE_DATA_TEST_HPP


#include "VoteData.hpp"
#include <assert.h>
#include <iostream>


class VoteDataTest {
public:
    static void testWriteVote();
    static void testWriteKey();
    static void testProcessHashes();
    static void testSuccessfulVote();
    static void testCheckExistingVotes();
};


#endif