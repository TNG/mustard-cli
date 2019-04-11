#include <vector>
#include <gtest/gtest.h>
#include <Provide.h>

using namespace testing;
using namespace std;

#ifndef BETSHEDOES_INITTESTS_H
#define BETSHEDOES_INITTESTS_H

class TestInitialiser
{
public:
    TestInitialiser ( int argc, char **argv );

    ~TestInitialiser() {}

    int runTests();

};

#endif //BETSHEDOES_INITTESTS_H
