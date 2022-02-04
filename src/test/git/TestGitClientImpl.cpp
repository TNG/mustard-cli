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
    testEnv.run ( "git checkout -q -b main" );
    testEnv.run ( "git checkout -q -b feature" );
    testEnv.createFileAndCommit ( "featureFile" );
    testEnv.run ( "git checkout -q main" );
    testEnv.createFileAndCommit ( "mainDevelopment" );
    EXPECT_STRNE ( expectedMergeBase.c_str(), gitClient.getHeadCommit().c_str() );

    const Commitish mergeBase = gitClient.getMergeBase ( "feature", "main" );

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

TEST_F ( TestGitClientImpl, Unit_GetConfigValue )
{
    GitTestEnvironment testEnv;
    const string expectedValue = "someValue";
    const string keyPath = "category.key";
    testEnv.run ( "git config --add " + keyPath + " " + expectedValue );

    const string valueFromConfig = gitClient.getConfigValue ( keyPath );

    EXPECT_STREQ ( expectedValue.c_str(), valueFromConfig.c_str() );
}

TEST_F ( TestGitClientImpl, Unit_GetDiff )
{
    GitTestEnvironment testEnv;
    testEnv.createFileAndCommit ( "myFile" );
    testEnv.run ( "echo someMoreContent >> myFile" );
    testEnv.run ( "git commit -m 'some more stuff'" );
    testEnv.run ( "echo lessContent >> myFile" );

    const string diff = gitClient.getDiff();
    const string expectedDiff = "diff --git a/myFile b/myFile\n"
                                "index e69de29..6924897 100644\n"
                                "--- a/myFile\n"
                                "+++ b/myFile\n"
                                "@@ -0,0 +1,2 @@\n"
                                "+someMoreContent\n"
                                "+lessContent\n"
                                "";

    EXPECT_STREQ ( expectedDiff.c_str(), diff.c_str() );
}

TEST_F ( TestGitClientImpl, Unit_getFeatureBranchOnOrigin )
{
    GitTestEnvironment testEnv;
    GitTestEnvironment remoteEnv ( "/tmp/remoteTestEnv" );
    testEnv.run ( "git remote add origin /tmp/remoteTestEnv" );
    testEnv.createFileAndCommit ( "testFile" );
    testEnv.run ( "git checkout -b feature 2>/dev/null" );
    testEnv.createFileAndCommit ( "anotherTestFile" );
    testEnv.run ( "git push -u origin HEAD 2>/dev/null" );
    const Commitish featureCommit ( gitClient.getHeadCommit() );
    testEnv.createFileAndCommit ( "yetAnotherTestFile" );
    ASSERT_EQ ( featureCommit, gitClient.getFeatureBranchOnOrigin() );
    ASSERT_NE ( featureCommit, gitClient.getHeadCommit() );
}

TEST_F ( TestGitClientImpl, Unit_getFeatureBranchOnOrigin_CommitMessageWithBrackets )
{
    GitTestEnvironment testEnv;
    GitTestEnvironment remoteEnv ( "/tmp/remoteTestEnv" );
    testEnv.run ( "git remote add origin /tmp/remoteTestEnv" );
    testEnv.createFileAndCommit ( "testFile" );
    testEnv.run ( "git checkout -b feature 2>/dev/null" );
    testEnv.run ( "touch anotherTestFile" );
    testEnv.run ( "git add anotherTestFile" );
    testEnv.run ( "git commit -m '[log] Another test file has been created [blah]'" );
    testEnv.run ( "git push -u origin HEAD 2>/dev/null" );
    const Commitish featureCommit ( gitClient.getHeadCommit() );
    ASSERT_EQ ( featureCommit, gitClient.getFeatureBranchOnOrigin() );
}
