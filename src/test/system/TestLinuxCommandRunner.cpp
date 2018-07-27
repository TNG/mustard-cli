#include <gtest/gtest.h>
#include "../../main/system/LinuxCommandRunner.h"

using namespace testing;

class TestLinuxCommandRunner : public Test
{
public:

    LinuxCommandRunner commandRunner;
};

TEST_F ( TestLinuxCommandRunner, Unit_GivesOutputAsString )
{
    EXPECT_STREQ (
        "Dies ist ein Test",
        commandRunner.run ( "echo -n Dies ist ein Test" ).getOutput().c_str()
    );
}

TEST_F ( TestLinuxCommandRunner, Unit_GivesOutputAsStringCanStripNewline )
{
    EXPECT_STREQ (
        "Dies ist ein Test",
        commandRunner.run ( "echo Dies ist ein Test" ).getOutputStripNewline().c_str()
    );
}

TEST_F ( TestLinuxCommandRunner, Unit_GivesCorrectExitCode )
{
    EXPECT_EQ (
        42,
        commandRunner.run ( "exit 42" ).getReturnCode()
    );
}
TEST_F ( TestLinuxCommandRunner, Unit_DoNotAcceptDoubleQuotesAtTheMoment )
{
    EXPECT_THROW ( commandRunner.run ( "echo Command With \" quote" ), exception );
}
