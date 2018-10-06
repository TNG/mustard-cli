#ifndef MUSTARD_AUTHENTICATIONPROVIDERIMPL_H
#define MUSTARD_AUTHENTICATIONPROVIDERIMPL_H

#include "AuthenticationProvider.h"
#include "../credentials/CredentialProvider.h"

class AuthenticationProviderImpl : public AuthenticationProvider
{
public:
    AuthenticationProviderImpl ( CredentialProvider *credentialProvider = nullptr );

    cpr::Authentication getAuthentication() override;

private:
    std::string username, password;
    bool hasCredentials = false;

    void askUserForCredentials();

    void toggleConsoleEcho();

    CredentialProvider *credentialProvider;

    void saveCredentials ( Credentials &credentials );
};


#endif //MUSTARD_AUTHENTICATIONPROVIDERIMPL_H
