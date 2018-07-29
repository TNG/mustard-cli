#include <iostream>
#include <termio.h>
#include <zconf.h>
#include <Provide.h>
#include "AuthenticationProviderImpl.h"

using namespace std;

ProvideImplementationForDependency<AuthenticationProviderImpl, AuthenticationProvider> authenticationProviderDependency;

cpr::Authentication AuthenticationProviderImpl::getAuthentication()
{

    if ( !hasCredentials ) {
        askUserForCredentials();
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
