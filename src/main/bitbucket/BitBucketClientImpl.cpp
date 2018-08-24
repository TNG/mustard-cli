#include <Depend.h>

#include <Provide.h>
#include "BitBucketClientImpl.h"
#include "BitBucketConfiguration.h"

using namespace rapidjson;

ProvideImplementationForDependency<BitBucketClientImpl, BitBucketClient> bitBucketClientDependency;

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
    checkForBitBucketErrors ( document );
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

void BitBucketClientImpl::checkForBitBucketErrors ( const rapidjson::Document &document )
{
    if ( document.HasMember ( "errors" ) ) {
        const string message = string ( ( *document["errors"].GetArray().Begin() ) ["message"].GetString() );
        throw BitBucketClientException ( ( "BitBucket server responded with error: " + message ).c_str() );
    }
}
