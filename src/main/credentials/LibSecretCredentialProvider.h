#ifndef MUSTARD_GNOMEKEYRINGCREDENTIALPROVIDER_H
#define MUSTARD_GNOMEKEYRINGCREDENTIALPROVIDER_H
#ifdef WITH_LIBSECRET

#include <libsecret/secret.h>
#include "CredentialProvider.h"
#include "../bitbucket/BitBucketConfiguration.h"


class LibSecretCredentialProvider : public CredentialProvider
{
public:
    LibSecretCredentialProvider ( BitBucketConfiguration *bitBucketConfiguration = nullptr );
    Credentials getCredentialsFor ( const string &serverName ) override;

    bool saveCredentials ( const Credentials &credentials ) override;

    bool isReady() override;

    ~LibSecretCredentialProvider() override;
private:
    BitBucketConfiguration *bitBucketConfiguration;
    static const SecretSchema secretSchema;
};

#endif
#endif //MUSTARD_GNOMEKEYRINGCREDENTIALPROVIDER_H
