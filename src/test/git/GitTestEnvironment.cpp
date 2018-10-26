#include <Depend.h>
#include "GitTestEnvironment.h"

GitTestEnvironment::GitTestEnvironment ( const string &baseDir, CommandRunner *commandRunner ) :
    baseDir ( baseDir ),
    commandRunner ( DependentOn<CommandRunner> ( commandRunner ) )
{
    CommandRunner *runner = this->commandRunner;
    runner->run ( "mkdir " + baseDir );
    pwd = runner->pwd();
    runner->cd ( baseDir );
    cout << runner->run ( "git init -q ." ).getOutput();
}

GitTestEnvironment::~GitTestEnvironment()
{
    commandRunner->cd ( pwd );
    commandRunner->run ( "rm -rf " + baseDir );
}

CommandResult GitTestEnvironment::run ( const string &cmd )
{
    commandRunner->cd ( baseDir );
    CommandResult result =  commandRunner->run ( cmd );
    return result;
}

void GitTestEnvironment::createFileAndCommit ( const string &file )
{
    commandRunner->cd ( baseDir );
    run ( "touch " + file );
    run ( "git add " + file );
    run ( "git commit -m '" + file + "'" );
}