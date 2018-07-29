#ifndef MUSTARD_BITBUCKETCLIENTIMPL_H
#define MUSTARD_BITBUCKETCLIENTIMPL_H

#include "BitBucketClient.h"
#include "../authentication/AuthenticationProvider.h"
#include "../system/HttpClient.h"

class BitBucketClientImpl : public BitBucketClient
{
public:
    BitBucketClientImpl ( HttpClient *httpClient = nullptr );
    Commitish getPullRequestTargetFor ( const Commitish &commit ) override;
private:
    HttpClient *httpClient;
};


#endif //MUSTARD_BITBUCKETCLIENTIMPL_H
