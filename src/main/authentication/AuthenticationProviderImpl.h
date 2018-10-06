#ifndef MUSTARD_AUTHENTICATIONPROVIDERIMPL_H
#define MUSTARD_AUTHENTICATIONPROVIDERIMPL_H

#include "AuthenticationProvider.h"
#include "../credentials/CredentialProvider.h"
#include "../bitbucket/BitBucketConfiguration.h"

class AuthenticationProviderImpl : public AuthenticationProvider
{
public:
    AuthenticationProviderImpl ( BitBucketConfiguration *bitBucketConfiguration = nullptr );

    cpr::Authentication getAuthentication() override;

private:
    std::string username, password;
    BitBucketConfiguration *bitBucketConfiguration;
};


#endif //MUSTARD_AUTHENTICATIONPROVIDERIMPL_H
