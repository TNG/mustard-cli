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
    Credentials askPersistAndReturnNewCredentials();
    virtual string getApiUrl() const;

private:
    GitClient *gitClient;

    CredentialProvider *credentialProvider;

    string getBitbucketUrl() const;

    string buildBitBucketUrl() const;

    void toggleConsoleEcho();

    Credentials askUserForCredentials();

    string getBitBucketServer() const;

    bool userHasSetVariable ( string variable );
};


#endif //MUSTARD_BITBUCKETCONFIGURATION_H
