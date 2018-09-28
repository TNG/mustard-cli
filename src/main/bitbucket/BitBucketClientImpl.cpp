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
    Document pullRequestsDocument = getPullRequestDocumentFor ( featureCommittish );
    Commitish committish ( extractPullRequestDocument ( pullRequestsDocument, featureCommittish ) ["toRef"]["latestCommit"].GetString() );
    return committish;
}

PullRequest BitBucketClientImpl::getPullRequestFor ( const Commitish &featureCommittish )
{
    Document pullRequestsDocument = getPullRequestDocumentFor ( featureCommittish );
    const string href = ( extractPullRequestDocument ( pullRequestsDocument, featureCommittish ) ["links"]["self"][0]["href"].GetString() );
    return {href};
}

rapidjson::Document BitBucketClientImpl::getPullRequestDocumentFor ( const Commitish &basic_string )
{
    const HttpResponse pullRequests = httpClient->get ( bitBucketEndpoint );
    if ( !pullRequests.successful ) {
        throw BitBucketClientException ( "Could not determine pull requests" );
    }
    Document document;
    document.Parse ( pullRequests.body.c_str() );
    checkForBitBucketErrors ( document );
    if ( !document.IsObject() ) {
        throw BitBucketClientException ( "Could not parse response json" );
    }
    return document;
}

const string BitBucketClientImpl::determineBitBucketEndpoint ( BitBucketConfiguration *config )
{
    return config->getBitBucketEndpoint();
}

const Document::ValueType &
BitBucketClientImpl::extractPullRequestDocument ( const Document &pullRequests, const Commitish &featureCommitish )
{
    for ( const auto &pullRequest : pullRequests["values"].GetArray() ) {
        if ( strcmp ( pullRequest["fromRef"]["latestCommit"].GetString(), featureCommitish.c_str() ) == 0 ) {
            return pullRequest;
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
