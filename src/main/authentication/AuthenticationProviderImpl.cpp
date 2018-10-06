#include <iostream>
#include <termio.h>
#include <zconf.h>
#include <Provide.h>
#include <Depend.h>
#include "AuthenticationProviderImpl.h"

using namespace std;

ProvideImplementationForDependency<AuthenticationProviderImpl, AuthenticationProvider> authenticationProviderDependency;

AuthenticationProviderImpl::AuthenticationProviderImpl ( CredentialProvider *credentialProvider ) :
    credentialProvider ( DependentOn<CredentialProvider> ( credentialProvider ) ) {}

cpr::Authentication AuthenticationProviderImpl::getAuthentication()
{
    if ( !hasCredentials ) {
        askUserForCredentials();
        Credentials credentials = {username, password};
        saveCredentials ( credentials );
    }
    return cpr::Authentication ( username.c_str(), password.c_str() );
}

void AuthenticationProviderImpl::askUserForCredentials()
{
    cout << "user:";
    cin >> username;
    cout << "password:";
    toggleConsoleEcho();
    cin.ignore();
    getline ( cin, password );
    toggleConsoleEcho();
    hasCredentials = true;
}

void AuthenticationProviderImpl::toggleConsoleEcho()
{
    termios oldt;
    tcgetattr ( STDIN_FILENO, &oldt );
    termios newt = oldt;
    newt.c_lflag &= ~ECHO;
    tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
}

void AuthenticationProviderImpl::saveCredentials ( Credentials &credentials )
{
    if ( credentialProvider->isReady() ) {
        credentialProvider->saveCredentials ( credentials );
    }
}
