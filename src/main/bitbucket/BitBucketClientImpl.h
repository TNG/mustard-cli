#ifndef MUSTARD_BITBUCKETCLIENTIMPL_H
#define MUSTARD_BITBUCKETCLIENTIMPL_H

#include "BitBucketClient.h"
#include "../authentication/AuthenticationProvider.h"
#include "../system/HttpClient.h"
#include "BitBucketConfiguration.h"

class BitBucketClientImpl : public BitBucketClient
{
public:
    BitBucketClientImpl ( HttpClient *httpClient, BitBucketConfiguration *bitBucketConfiguration );
    Commitish getPullRequestTargetFor ( const Commitish &commit ) override;
private:
    HttpClient *httpClient;
    const string bitBucketEndpoint;

    const string determineBitBucketEndpoint ( BitBucketConfiguration *config );
};


#endif //MUSTARD_BITBUCKETCLIENTIMPL_H
