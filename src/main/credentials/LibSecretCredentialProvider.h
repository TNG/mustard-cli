#ifndef MUSTARD_GNOMEKEYRINGCREDENTIALPROVIDER_H
#define MUSTARD_GNOMEKEYRINGCREDENTIALPROVIDER_H
#ifdef WITH_LIBSECRET

#include <libsecret/secret.h>
#include "CredentialProvider.h"
#include "../bitbucket/BitBucketConfiguration.h"


class LibSecretCredentialProvider : public CredentialProvider
{
public:
    string getPasswordFor ( const string &serverName, const string &userName ) override;

    bool saveCredentials ( const string &serverName, const Credentials &credentials ) override;

    bool isReady() override;

    ~LibSecretCredentialProvider() override;
private:
    static const SecretSchema secretSchema;
};

#endif
#endif //MUSTARD_GNOMEKEYRINGCREDENTIALPROVIDER_H
