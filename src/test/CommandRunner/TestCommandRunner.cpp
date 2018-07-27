#include <gtest/gtest.h>
#include "../../main/system/CommandRunner.h"

using namespace testing;

class TestCommandRunner : public Test
{
public:

    CommandRunner commandRunner;
};

TEST_F ( TestCommandRunner, Unit_GivesOutputAsString )
{
    EXPECT_STREQ (
        "Dies ist ein Test",
        commandRunner.run ( "echo -n Dies ist ein Test" ).getOutput().c_str()
    );
}

TEST_F ( TestCommandRunner, Unit_GivesCorrectExitCode )
{
    EXPECT_EQ (
        42,
        commandRunner.run ( "exit 42" ).getReturnCode()
    );
}
TEST_F ( TestCommandRunner, Unit_DoNotAcceptDoubleQuotesAtTheMoment )
{
    EXPECT_THROW ( commandRunner.run ( "echo Command With \" quote" ), exception );
}
