#include <Depend.h>
#include "GitTestEnvironment.h"

GitTestEnvironment::GitTestEnvironment ( CommandRunner *commandRunner ) :
    commandRunner ( DependentOn<CommandRunner> ( commandRunner ) )
{
    CommandRunner *runner = this->commandRunner;
    runner->run ( "mkdir /tmp/testEnv" );
    pwd = runner->pwd();
    runner->cd ( "/tmp/testEnv" );
    cout << runner->run ( "git init -q ." ).getOutput();

}

GitTestEnvironment::~GitTestEnvironment()
{
    commandRunner->cd ( pwd );
    commandRunner->run ( "rm -rf /tmp/testEnv" );
}

CommandResult GitTestEnvironment::run ( const string &cmd )
{
    return commandRunner->run ( cmd );
}

void GitTestEnvironment::createFileAndCommit ( const string &file )
{
    run ( "touch " + file );
    run ( "git add " + file );
    run ( "git commit -m '" + file + "'" );
}