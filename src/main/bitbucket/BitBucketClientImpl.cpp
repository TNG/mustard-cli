#include <cpr/cpr.h>
#include <Depend.h>
#include "BitBucketClientImpl.h"
#include "BitBucketConfiguration.h"

BitBucketClientImpl::BitBucketClientImpl ( HttpClient *httpClient, BitBucketConfiguration *bitBucketConfiguration ) :
    httpClient ( DependentOn<HttpClient> ( httpClient ) ),
    bitBucketEndpoint ( determineBitBucketEndpoint ( DependentOn<BitBucketConfiguration> ( bitBucketConfiguration ) ) )
{
}

Commitish BitBucketClientImpl::getPullRequestTargetFor ( const Commitish &commit )
{
    const HttpResponse pullRequest = httpClient->get ( bitBucketEndpoint );
    if ( !pullRequest.successful ) {
        printf ( "Could not determine pull requests\n" );
    }
    printf ( "%d - '%s'\n",  pullRequest.httpStatus, pullRequest.body.c_str() );
    return Commitish();
}

const string BitBucketClientImpl::determineBitBucketEndpoint ( BitBucketConfiguration *config )
{
    return config->getBitBucketEndpoint();
}
