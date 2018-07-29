#ifndef MUSTARD_BITBUCKETCLIENTIMPL_H
#define MUSTARD_BITBUCKETCLIENTIMPL_H

#include "BitBucketClient.h"
#include "../authentication/AuthenticationProvider.h"

class BitBucketClientImpl : public BitBucketClient
{
public:
    BitBucketClientImpl ( AuthenticationProvider *authenticationProvider = nullptr );
    Commitish getPullRequestTargetFor ( const Commitish &commit ) override;
private:
    AuthenticationProvider *authenticationProvider;
};


#endif //MUSTARD_BITBUCKETCLIENTIMPL_H
