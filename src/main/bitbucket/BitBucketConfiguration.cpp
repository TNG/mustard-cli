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
    const string bitbucketUrl = gitClient->getConfigValue ( "mustard.bitbucket" );
    const string repositorySlug = gitClient->getConfigValue ( "mustard.repositorySlug" );
    const string projectKey = gitClient->getConfigValue ( "mustard.projectKey" );

    stringstream ss;
    ss << bitbucketUrl << "/rest/api/1.0/projects/" << projectKey << "/repos/" << repositorySlug;
    return ss.str();
}

Credentials BitBucketConfiguration::getCredentials()
{
    const string userName = gitClient->getConfigValue ( "mustard.userName" );
    const string password = credentialProvider->getPasswordFor ( getBitbucketUrl(), userName );
    if ( password.empty() ) {
        return askPersistAndReturnNewCredentials();
    }
    return {userName, password};
}

Credentials BitBucketConfiguration::askPersistAndReturnNewCredentials()
{
    Credentials toPersist = askUserForCredentials();
    credentialProvider->saveCredentials ( getBitbucketUrl(), toPersist );
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
