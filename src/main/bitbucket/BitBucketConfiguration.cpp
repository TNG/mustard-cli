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
        stringstream ss;
        ss << getBitbucketUrl();
        ss << "/pull-requests";
        return ss.str();
    } catch ( GitClientException exception ) {
        throw MustardException (
            ( "Could not determine BitBucket endpoint url for this repository. Reason: "
              + string ( exception.what() )
            ).c_str() );
    }
}

string BitBucketConfiguration::getBitbucketUrl() const
{
    static string bitBucketUrl = buildBitBucketUrl();
    return bitBucketUrl;
}

string BitBucketConfiguration::buildBitBucketUrl() const
{
    const string bitbucketUrl = gitClient->getConfigValue ( "mustard.bitbucket" );
    const string repositorySlug = gitClient->getConfigValue ( "mustard.repositorySlug" );
    const string projectKey = gitClient->getConfigValue ( "mustard.projectKey" );

    stringstream ss;
    ss << bitbucketUrl << "/rest/api/1.0/projects/" << projectKey << "/repos/" << repositorySlug;
    return ss.str();
}
