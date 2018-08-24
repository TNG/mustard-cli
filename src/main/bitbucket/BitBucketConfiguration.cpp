#include <Depend.h>
#include <Provide.h>
#include "BitBucketConfiguration.h"

ProvideDependency<BitBucketConfiguration> bitBucketConfigurationDependency;

BitBucketConfiguration::BitBucketConfiguration ( GitClient *gitClient ) :
    gitClient ( DependentOn<GitClient> ( gitClient ) )
{}

const string BitBucketConfiguration::getBitBucketEndpoint()
{
    return gitClient->getConfigValue ( "mustard.bitbucketurl" );
}
