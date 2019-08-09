#include <Provide.h>
#include <Depend.h>
#include "PasswdWorkflow.h"

ProvideDependency<PasswdWorkflow> passwdWorkflowDependency;

PasswdWorkflow::PasswdWorkflow ( BitBucketConfiguration *bitBucketConfiguration ) :
    configuration ( DependentOn<BitBucketConfiguration> ( bitBucketConfiguration ) ) {}

int PasswdWorkflow::run ( int argc, const char **argv )
{
    configuration->askPersistAndReturnNewCredentials();
    return 0;
}
