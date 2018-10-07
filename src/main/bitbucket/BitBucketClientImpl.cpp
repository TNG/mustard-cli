#include <Depend.h>

#include <Provide.h>
#include "BitBucketClientImpl.h"
#include "BitBucketConfiguration.h"

using namespace rapidjson;

ProvideImplementationForDependency<BitBucketClientImpl, BitBucketClient> bitBucketClientDependency;

BitBucketClientImpl::BitBucketClientImpl ( HttpClient *httpClient, BitBucketConfiguration *bitBucketConfiguration ) :
    httpClient ( DependentOn<HttpClient> ( httpClient ) ),
    pullRequestEndpoint ( determinePullRequestEndpoint ( DependentOn<BitBucketConfiguration> ( bitBucketConfiguration ) ) )
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
    auto &pullRequestDocument =  extractPullRequestDocument ( pullRequestsDocument, featureCommittish );
    const string href ( pullRequestDocument ["links"]["self"][0]["href"].GetString() );
    const unsigned int id = pullRequestDocument ["id"].GetInt();
    const string title ( pullRequestDocument ["title"].GetString() );
    const string description ( pullRequestDocument ["description"].GetString() );
    return {href,
            id,
            title,
            description,
            User::from ( pullRequestDocument["author"]["user"] ),
            extractReviewersFrom ( pullRequestDocument["reviewers"] )
           };
}

vector<Reviewer> BitBucketClientImpl::extractReviewersFrom ( const Document::ValueType &reviewersArray )
{
    vector<Reviewer> reviewers;
    for ( const auto &reviewer : reviewersArray.GetArray() ) {
        reviewers.push_back ( Reviewer::from (
                                  User::from ( reviewer["user"] ),
                                  reviewer["status"].GetString()
                              ) );
    }
    return reviewers;
}

rapidjson::Document BitBucketClientImpl::getPullRequestDocumentFor ( const Commitish &basic_string )
{
    const HttpResponse pullRequests = httpClient->get ( pullRequestEndpoint );
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

const string BitBucketClientImpl::determinePullRequestEndpoint ( BitBucketConfiguration *config )
{
    return config->getPullRequestEndpoint();
}

const Document::ValueType &
BitBucketClientImpl::extractPullRequestDocument ( const Document &pullRequests, const Commitish &featureCommitish )
{
    for ( const auto &pullRequest : pullRequests["values"].GetArray() ) {
        if ( strcmp ( pullRequest["fromRef"]["latestCommit"].GetString(), featureCommitish.c_str() ) == 0 ) {
            return pullRequest;
        };
    }
    throw BitBucketClientException ( "Could not find pullRequest for given feature branch.\n"
                                     "Maybe feature branch is outdated?" );
}

void BitBucketClientImpl::checkForBitBucketErrors ( const rapidjson::Document &document )
{
    if ( document.HasMember ( "errors" ) ) {
        const string message = string ( ( *document["errors"].GetArray().Begin() ) ["message"].GetString() );
        throw BitBucketClientException ( ( "BitBucket server responded with error: " + message ).c_str() );
    }
}
