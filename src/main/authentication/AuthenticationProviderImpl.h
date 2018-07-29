#ifndef MUSTARD_AUTHENTICATIONPROVIDERIMPL_H
#define MUSTARD_AUTHENTICATIONPROVIDERIMPL_H

#include "AuthenticationProvider.h"

class AuthenticationProviderImpl : public AuthenticationProvider
{
public:
    cpr::Authentication getAuthentication() override;

private:
    std::string username, password;
    bool hasCredentials = false;

    void askUserForCredentials();

    void toggleConsoleEcho();
};


#endif //MUSTARD_AUTHENTICATIONPROVIDERIMPL_H
