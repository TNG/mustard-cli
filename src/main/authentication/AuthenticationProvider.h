#ifndef MUSTARD_AUTHENTICATIONPROVIDER_H
#define MUSTARD_AUTHENTICATIONPROVIDER_H
#include <cpr/cpr.h>

class AuthenticationProvider
{
public:
    virtual cpr::Authentication getAuthentication() = 0;
};

#endif //MUSTARD_AUTHENTICATIONPROVIDER_H
