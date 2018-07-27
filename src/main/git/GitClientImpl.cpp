#include <Depend.h>
#include "GitClientImpl.h"

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
    commandRunner->run ( "git reset " + resetTo );
}

string GitClientImpl::getConfigValue ( const string &name )
{
    return commandRunner->run ( "git config --get " + name ).getOutputStripNewline();
}
