#include <Depend.h>
#include <Provide.h>
#include "BitBucketConfiguration.h"
#include "../git/GitClientException.h"

ProvideDependency<BitBucketConfiguration> bitBucketConfigurationDependency;

BitBucketConfiguration::BitBucketConfiguration ( GitClient *gitClient ) :
    gitClient ( DependentOn<GitClient> ( gitClient ) ) {}

const string BitBucketConfiguration::getBitBucketEndpoint()
{
    try {
        const string endpoint;
        return gitClient->getConfigValue ( "mustard.bitbucketurl" );
    } catch ( GitClientException exception ) {
        throw MustardException (
            ( "Could not determine BitBucket endpoint url for this repository. Reason: "
              + string ( exception.what() )
            ).c_str() );
    }
}