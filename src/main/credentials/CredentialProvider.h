#ifndef MUSTARD_CREDENTIALPROVIDER_H
#define MUSTARD_CREDENTIALPROVIDER_H

#include "Credentials.h"
#include "../error/MustardException.h"

class CredentialProvider
{
public:
    virtual Credentials getCredentialsFor ( const string &serverName ) {
        throw MustardException ( "Cannot get credentials as no CredentialProvider has been compiled" );
    }

    virtual bool saveCredentials ( const Credentials &credentials ) {
        return false;
    }

    virtual bool isReady() {
        return false;
    }

    virtual ~CredentialProvider() {}
};

#endif //MUSTARD_CREDENTIALPROVIDER_H
