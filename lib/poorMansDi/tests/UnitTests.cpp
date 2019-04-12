#include <gtest/gtest.h>
#include "TestFilterHandling.h"
#include "InitTests.h"

using namespace testing;

/**
 * @brief Main method to run all unit tests registered with google test
 */
int main ( int argc, char **argv )
{
    TestInitialiser initTests ( argc, argv );

    string generalFilter = "*.Unit_*:*.DISABLED_Unit_*";
    string customFilter = ::testing::GTEST_FLAG ( filter );
    ::testing::GTEST_FLAG ( filter ) = combineFilter ( generalFilter, customFilter );

    return initTests.runTests();
}

