#ifndef MUSTARD_GNOMEKEYRINGCREDENTIALPROVIDER_H
#define MUSTARD_GNOMEKEYRINGCREDENTIALPROVIDER_H
#ifdef WITH_GNOME_KEYRING
#include "CredentialProvider.h"
#include "../bitbucket/BitBucketConfiguration.h"


class GnomeKeyringCredentialProvider : public CredentialProvider
{
public:
    GnomeKeyringCredentialProvider ( BitBucketConfiguration *bitBucketConfiguration = nullptr );
    Credentials getCredentialsFor ( const string &serverName ) override;

    bool saveCredentials ( const Credentials &credentials ) override;

    bool isReady() override;

    ~GnomeKeyringCredentialProvider() override;
private:
    BitBucketConfiguration *bitBucketConfiguration;
};

#endif
#endif //MUSTARD_GNOMEKEYRINGCREDENTIALPROVIDER_H
