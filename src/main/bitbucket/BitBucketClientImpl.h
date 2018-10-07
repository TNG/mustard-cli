#ifndef MUSTARD_BITBUCKETCLIENTIMPL_H
#define MUSTARD_BITBUCKETCLIENTIMPL_H

#include "BitBucketClientException.h"
#define RAPIDJSON_ASSERT(x) if (!(x)) throw BitBucketClientException( "Error when parsing json" )
#include <rapidjson/document.h>

#include "BitBucketClient.h"
#include "../authentication/AuthenticationProvider.h"
#include "../system/HttpClient.h"
#include "BitBucketConfiguration.h"

using namespace rapidjson;

class BitBucketClientImpl : public BitBucketClient
{
public:
    BitBucketClientImpl ( HttpClient *httpClient = nullptr, BitBucketConfiguration *bitBucketConfiguration = nullptr );
    Commitish getPullRequestTargetFor ( const Commitish &featureCommittish ) override;
    PullRequest getPullRequestFor ( const Commitish &featureCommittish ) override;
private:
    HttpClient *httpClient;
    const string pullRequestEndpoint;

    const string determinePullRequestEndpoint ( BitBucketConfiguration *config );

    const Document::ValueType &
    extractPullRequestDocument ( const rapidjson::Document &pullRequest, const Commitish &featureCommitish );

    void checkForBitBucketErrors ( const rapidjson::Document &document );

    rapidjson::Document getPullRequestDocumentFor ( const Commitish &basic_string );

    vector<Reviewer> extractReviewersFrom ( const Document::ValueType &reviewersArray );
};


#endif //MUSTARD_BITBUCKETCLIENTIMPL_H
