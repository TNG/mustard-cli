#include <gtest/gtest.h>
#include <Depend.h>
#include "../../main/git/GitClientImpl.h"

using  namespace testing;

class GitTestEnvironment
{
public:
    GitTestEnvironment ( CommandRunner *commandRunner = nullptr ) :
        commandRunner ( DependentOn<CommandRunner> ( commandRunner ) ) {
        CommandRunner *runner = this->commandRunner;
        runner->run ( "mkdir /tmp/testEnv" );
        pwd = runner->pwd();
        runner->cd ( "/tmp/testEnv" );
        cout << runner->run ( "git init -q ." ).getOutput();

    }
    ~GitTestEnvironment() {
        commandRunner->cd ( pwd );
        commandRunner->run ( "rm -rf /tmp/testEnv" );
    }
    CommandResult run ( const string &cmd ) {
        return commandRunner->run ( cmd );
    }
    void createFileAndCommit ( const string &file ) {
        run ( "touch " + file );
        run ( "git add " + file );
        run ( "git commit -m '" + file + "'" );
    }
private:
    CommandRunner *commandRunner;
    string pwd;

};

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

