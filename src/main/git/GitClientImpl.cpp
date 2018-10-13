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
        throw GitClientException ( ( "Could not get config value for " + name + ". Maybe run 'mustard autoconf ?'" ).c_str() );
    }
    return commandResult.getOutputStripNewline();
}

void GitClientImpl::setConfigValue ( const string &name, const string &value )
{
    CommandResult commandResult =  commandRunner->run ( "git config " + name + " " + value );
    if ( commandResult.getReturnCode() ) {
        throw GitClientException ( ( "Could not set config value for " + name ).c_str() );
    }
}

string GitClientImpl::getDiff()
{
    CommandResult commandResult =  commandRunner->run ( "git diff -U0" );
    if ( commandResult.getReturnCode() ) {
        throw GitClientException ( ( "Could not get diff, git returned:" + commandResult.getOutput() ).c_str() );
    }
    return commandResult.getOutput();
}

Commitish GitClientImpl::getFeatureBranchOnOrigin()
{
    CommandResult commandResult = commandRunner->run ( "git rev-parse $(git branch -vv|grep \\*|sed -r 's/.*\\[([^ :]*).*\\].*/\\1/')" );
    if ( commandResult.getReturnCode() != 0 ) {
        throw GitClientException ( "Could not determine feature branch head - maybe you are on a non tracking branch?" );
    }
    return Commitish ( commandResult.getOutputStripNewline() );
}
