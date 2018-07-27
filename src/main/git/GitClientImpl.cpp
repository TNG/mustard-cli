#include <Depend.h>
#include "GitClientImpl.h"

GitClientImpl::GitClientImpl ( CommandRunner *commandRunner ) :
    commandRunner ( DependentOn<CommandRunner> ( commandRunner ) ) {}

ProvideImplementationForDependency<GitClientImpl, GitClient> gitClientDependency;

Commitish GitClientImpl::getHeadCommit()
{
    return commandRunner->run ( "git rev-parse HEAD" ).getOutputStripNewline();
}
