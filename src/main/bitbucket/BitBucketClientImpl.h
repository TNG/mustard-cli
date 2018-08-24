#ifndef MUSTARD_BITBUCKETCLIENTIMPL_H
#define MUSTARD_BITBUCKETCLIENTIMPL_H

#include "BitBucketClientException.h"
#define RAPIDJSON_ASSERT(x) if (!(x)) throw BitBucketClientException( "Error when parsing json" )
#include <rapidjson/document.h>

#include "BitBucketClient.h"
#include "../authentication/AuthenticationProvider.h"
#include "../system/HttpClient.h"
#include "BitBucketConfiguration.h"

class BitBucketClientImpl : public BitBucketClient
{
public:
    BitBucketClientImpl ( HttpClient *httpClient = nullptr, BitBucketConfiguration *bitBucketConfiguration = nullptr );
    Commitish getPullRequestTargetFor ( const Commitish &featureCommittish ) override;
private:
    HttpClient *httpClient;
    const string bitBucketEndpoint;

    const string determineBitBucketEndpoint ( BitBucketConfiguration *config );

    Commitish extractPullRequestTargetFrom ( const string &pullRequestsJson, const Commitish &featureCommitish );

    void checkForBitBucketErrors ( const rapidjson::Document &document );
};


#endif //MUSTARD_BITBUCKETCLIENTIMPL_H
