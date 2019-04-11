#include <gtest/gtest.h>
#include "InitTests.h"

using namespace testing;

/**
 * @brief Main method to run all tests registered with google test
 */
int main ( int argc, char **argv )
{
    TestInitialiser initTests ( argc, argv );

    return initTests.runTests();
}



