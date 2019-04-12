#ifndef TESTFILTERHANDLING_H
#define TESTFILTERHANDLING_H

#include <string>
#include <vector>
#include <istream>
#include <sstream>

using namespace std;

/**
 * @brief Helper function to combine test filters for gtest
 * @param generalFilter The general filter for the current test setup
 * @param customFilter A user set custom filter to be applied together with the general one
 * @return The total filter
 * @note Despite the file name this is not a test itself!
 */
string combineFilter ( const string &generalFilter, const string &customFilter );

#endif // TESTFILTERHANDLING_H

