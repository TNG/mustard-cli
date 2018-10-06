#ifndef MUSTARD_CREDENTIALPROVIDER_H
#define MUSTARD_CREDENTIALPROVIDER_H

#include "Credentials.h"
#include "../error/MustardException.h"

class CredentialProvider
{
public:
    virtual string getPasswordFor ( const string &serverName, const string &userName ) {
        throw MustardException ( "Cannot get credentials as no CredentialProvider has been compiled" );
    }

    virtual bool saveCredentials ( const string &serverName, const Credentials &credentials ) {
        return false;
    }

    virtual bool isReady() {
        return false;
    }

    virtual ~CredentialProvider() {}
};

#endif //MUSTARD_CREDENTIALPROVIDER_H
