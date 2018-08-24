#include <Depend.h>
#include "GitClientImpl.h"
#include "GitClientException.h"

GitClientImpl::GitClientImpl ( CommandRunner *commandRunner ) :
    commandRunner ( DependentOn<CommandRunner> ( commandRunner ) ) {}

ProvideImplementationForDependency<GitClientImpl, GitClient> gitClientDependency;

Commitish GitClientImpl::getHeadCommit()
{
    return commandRunner->run ( "git rev-parse HEAD" ).getOutputStripNewline();
}

bool GitClientImpl::workingDirectoryIsClean()
{
    return commandRunner->run ( "git status --porcelain" ).getOutput().length() == 0;
}

Commitish GitClientImpl::getMergeBase ( const Commitish &fromBranch, const Commitish &toBranch )
{
    return commandRunner->run ( "git merge-base " + fromBranch + " " + toBranch ).getOutputStripNewline();
}

void GitClientImpl::reset ( const Commitish &resetTo )
{
    CommandResult resetResult = commandRunner->run ( "git reset " + resetTo );
    if ( resetResult.getReturnCode() ) {
        throw GitClientException ( resetResult.getOutput().c_str() );
    }
}

string GitClientImpl::getConfigValue ( const string &name )
{
    CommandResult commandResult =  commandRunner->run ( "git config --get " + name );
    if ( commandResult.getReturnCode() ) {
        throw GitClientException ( ( "Could not get config value for " + name ).c_str() );
    }
    return commandResult.getOutputStripNewline();
}
