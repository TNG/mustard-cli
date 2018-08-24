#include "BitBucketClientException.h"

#define RAPIDJSON_ASSERT(x) if (!(x)) throw BitBucketClientException(RAPIDJSON_STRINGIFY(x))
#include <rapidjson/document.h>

#include <Depend.h>
#include "BitBucketClientImpl.h"
#include "BitBucketConfiguration.h"

using namespace rapidjson;

BitBucketClientImpl::BitBucketClientImpl ( HttpClient *httpClient, BitBucketConfiguration *bitBucketConfiguration ) :
    httpClient ( DependentOn<HttpClient> ( httpClient ) ),
    bitBucketEndpoint ( determineBitBucketEndpoint ( DependentOn<BitBucketConfiguration> ( bitBucketConfiguration ) ) )
{
}

Commitish BitBucketClientImpl::getPullRequestTargetFor ( const Commitish &featureCommittish )
{
    const HttpResponse pullRequests = httpClient->get ( bitBucketEndpoint );
    if ( !pullRequests.successful ) {
        throw BitBucketClientException ( "Could not determine pull requests" );
    }
    return extractPullRequestTargetFrom ( pullRequests.body, featureCommittish );
}

const string BitBucketClientImpl::determineBitBucketEndpoint ( BitBucketConfiguration *config )
{
    return config->getBitBucketEndpoint();
}

Commitish
BitBucketClientImpl::extractPullRequestTargetFrom ( const string &pullRequestsJson, const Commitish &featureCommitish )
{
    Document document;
    document.Parse ( pullRequestsJson.c_str() );
    if ( !document.IsObject() ) {
        throw BitBucketClientException ( "Could not parse response json" );
    }
    for ( const auto &pullRequest : document["values"].GetArray() ) {
        if ( strcmp ( pullRequest["fromRef"]["latestCommit"].GetString(), featureCommitish.c_str() ) == 0 ) {
            return Commitish ( pullRequest["toRef"]["latestCommit"].GetString() );
        };
    }
    throw BitBucketClientException ( "Could not find pullRequest target for feature committish" );
}
