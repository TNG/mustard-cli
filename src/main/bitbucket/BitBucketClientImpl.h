#ifndef MUSTARD_BITBUCKETCLIENTIMPL_H
#define MUSTARD_BITBUCKETCLIENTIMPL_H

#include "BitBucketClient.h"

using namespace Pistache;

class BitBucketClientImpl : public BitBucketClient
{
public:
    BitBucketClientImpl();
    Commitish getPullRequestTargetFor ( const Commitish &commit ) override;
    ~BitBucketClientImpl() {
        client.shutdown();
    }
private:
    Http::Client client;


};


#endif //MUSTARD_BITBUCKETCLIENTIMPL_H
