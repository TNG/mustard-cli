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
    const string title ( pullRequestDocument["title"].GetString() );
    const string description ( pullRequestDocument.HasMember ( "description" )
                               ? pullRequestDocument["description"].GetString()
                               : "" );
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
    return getDocument ( pullRequests );
}

Document BitBucketClientImpl::getDocument ( const HttpResponse &pullRequests )
{
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

Comments BitBucketClientImpl::getCommentsFor ( const PullRequest &pullRequest )
{
    stringstream activitiesUrl;
    activitiesUrl << pullRequestEndpoint << "/" << pullRequest.id << "/activities?limit=1000" ;
    const string url = activitiesUrl.str();

    const HttpResponse activitiesResponse = httpClient->get ( url );
    if ( !activitiesResponse.successful ) {
        throw BitBucketClientException ( "Could not retrieve activities for pullRequest." );
    }

    Document activities = getDocument ( activitiesResponse );
    return extractCommentsFrom ( activities );
}

Comments BitBucketClientImpl::extractCommentsFrom ( Document &document )
{
    map<string, vector<LineComment>> commentsFromBitBucket;
    for ( const auto &value : document["values"].GetArray() ) {
        if ( value["action"].GetString() != string ( "COMMENTED" ) || value["commentAction"].GetString() != string ( "ADDED" ) ) {
            continue;
        }
        const auto &comment = value["comment"];
        const string author = comment["author"]["name"].GetString();
        const string text =  comment["text"].GetString();
        const auto &commentAnchor = value["commentAnchor"];
        if ( commentAnchor["lineType"].GetString() != string ( "ADDED" ) ) {
            continue;
        }
        const auto line = ( unsigned int ) commentAnchor["line"].GetInt();
        const string path = commentAnchor["path"].GetString();
        commentsFromBitBucket[path].push_back ( {line, text, author} );
    }
    vector<FileComments> fileComments;
    for ( const auto &commentFromBitBucket : commentsFromBitBucket ) {
        fileComments.push_back ( {commentFromBitBucket.first, commentFromBitBucket.second} );
    }
    return Comments ( fileComments );
}
