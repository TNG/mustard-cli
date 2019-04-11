#include "InitTests.h"

TestInitialiser::TestInitialiser ( int argc, char **argv )
{
    ::testing::InitGoogleTest ( &argc, argv );
    ::testing::GTEST_FLAG ( shuffle ) =  true;
}

int TestInitialiser::runTests()
{
    return RUN_ALL_TESTS();
}
