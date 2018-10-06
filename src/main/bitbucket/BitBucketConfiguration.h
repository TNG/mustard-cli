#ifndef MUSTARD_BITBUCKETCONFIGURATION_H
#define MUSTARD_BITBUCKETCONFIGURATION_H


#include "../git/GitClient.h"
#include "../credentials/CredentialProvider.h"

class BitBucketConfiguration
{
public:
    BitBucketConfiguration ( GitClient *gitClient = nullptr, CredentialProvider *credentialProvider = nullptr );
    virtual const string getPullRequestEndpoint();
    virtual Credentials getCredentials();

private:

    GitClient *gitClient;
    CredentialProvider *credentialProvider;


    string getBitbucketUrl() const;

    string buildBitBucketUrl() const;

    Credentials askPersistAndReturnNewCredentials();

    void toggleConsoleEcho();

    Credentials askUserForCredentials();
};


#endif //MUSTARD_BITBUCKETCONFIGURATION_H
