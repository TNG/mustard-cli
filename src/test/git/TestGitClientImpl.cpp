#include <gtest/gtest.h>
#include <Depend.h>
#include "../../main/git/GitClientImpl.h"
#include "GitTestEnvironment.h"

using  namespace testing;

class TestGitClientImpl: public Test
{
public:
    GitClientImpl gitClient;
};

TEST_F ( TestGitClientImpl, Unit_GetHeadCommit )
{
    GitTestEnvironment testEnv;

    testEnv.createFileAndCommit ( "test" );

    EXPECT_TRUE ( gitClient.workingDirectoryIsClean() );
}

TEST_F ( TestGitClientImpl, Unit_WorkingDirIsClean_No )
{
    GitTestEnvironment testEnv;
    testEnv.createFileAndCommit ( "test" );

    testEnv.run ( "touch anotherone" );

    EXPECT_FALSE ( gitClient.workingDirectoryIsClean() );
}

TEST_F ( TestGitClientImpl, Unit_FindMergeBase )
{
    GitTestEnvironment testEnv;
    testEnv.createFileAndCommit ( "test" );
    const Commitish expectedMergeBase = gitClient.getHeadCommit();
    testEnv.run ( "git checkout -q -b feature" );
    testEnv.createFileAndCommit ( "featureFile" );
    testEnv.run ( "git checkout -q master" );
    testEnv.createFileAndCommit ( "masterDevelopment" );
    EXPECT_STRNE ( expectedMergeBase.c_str(), gitClient.getHeadCommit().c_str() );

    const Commitish mergeBase = gitClient.getMergeBase ( "feature", "master" );

    EXPECT_STREQ ( expectedMergeBase.c_str(), mergeBase.c_str() );
}

TEST_F ( TestGitClientImpl, Unit_GitReset )
{
    GitTestEnvironment testEnv;
    testEnv.createFileAndCommit ( "someFile" );
    testEnv.createFileAndCommit ( "someOtherFile" );
    EXPECT_TRUE ( gitClient.workingDirectoryIsClean() );
    testEnv.run ( "git branch future" );
    gitClient.reset ( "HEAD^" );
    testEnv.run ( "touch someOtherFile" );
    EXPECT_FALSE ( gitClient.workingDirectoryIsClean() );

    gitClient.reset ( "future" );

    EXPECT_TRUE ( gitClient.workingDirectoryIsClean() );
}

