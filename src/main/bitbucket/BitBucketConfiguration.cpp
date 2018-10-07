#include <Depend.h>
#include <Provide.h>
#include <termios.h>
#include "BitBucketConfiguration.h"
#include "../git/GitClientException.h"

ProvideDependency<BitBucketConfiguration> bitBucketConfigurationDependency;

BitBucketConfiguration::BitBucketConfiguration ( GitClient *gitClient, CredentialProvider *credentialProvider ) :
    gitClient ( DependentOn<GitClient> ( gitClient ) ),
    credentialProvider ( DependentOn<CredentialProvider> ( credentialProvider ) )
{}

const string BitBucketConfiguration::getPullRequestEndpoint()
{
    try {
        stringstream ss;
        ss << getBitbucketUrl();
        ss << "/pull-requests";
        return ss.str();
    } catch ( GitClientException exception ) {
        throw MustardException (
            ( "Could not determine BitBucket endpoint url for this repository. Reason: "
              + string ( exception.what() )
            ).c_str() );
    }
}

string BitBucketConfiguration::getBitbucketUrl() const
{
    static string bitBucketUrl = buildBitBucketUrl();
    return bitBucketUrl;
}

string BitBucketConfiguration::buildBitBucketUrl() const
{
    string bitbucketUrl = getBitBucketServer();
    const string repositorySlug = gitClient->getConfigValue ( "mustard.repositorySlug" );
    const string projectKey = gitClient->getConfigValue ( "mustard.projectKey" );

    stringstream ss;
    ss << bitbucketUrl << "/rest/api/1.0/projects/" << projectKey << "/repos/" << repositorySlug;
    return ss.str();
}

string BitBucketConfiguration::getBitBucketServer() const
{
    static const string bitbucketUrl = gitClient->getConfigValue ( "mustard.bitbucket" );
    return bitbucketUrl;
}

Credentials BitBucketConfiguration::getCredentials()
{
    string userName;
    try {
        userName = gitClient->getConfigValue ( "mustard.userName" );
    } catch ( GitClientException &e ) {
        Credentials newCredentials = askPersistAndReturnNewCredentials();
        return newCredentials;
    }
    const string password = credentialProvider->getPasswordFor ( getBitBucketServer(), userName );
    if ( password.empty() ) {
        Credentials newCredentials = askPersistAndReturnNewCredentials();
        return newCredentials;
    }
    return {userName, password};
}

Credentials BitBucketConfiguration::askPersistAndReturnNewCredentials()
{
    Credentials toPersist = askUserForCredentials();
    gitClient->setConfigValue ( "mustard.userName", toPersist.username );
    credentialProvider->saveCredentials ( getBitBucketServer(), toPersist );
    return toPersist;
}

Credentials BitBucketConfiguration::askUserForCredentials()
{
    string username, password;
    cout << "user:";
    cin >> username;
    cout << "password:";
    toggleConsoleEcho();
    cin.ignore();
    getline ( cin, password );
    toggleConsoleEcho();
    return {username, password};
}

void BitBucketConfiguration::toggleConsoleEcho()
{
    termios oldt;
    tcgetattr ( STDIN_FILENO, &oldt );
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
}
