#include <iostream>
#include <Provide.h>
#include <Depend.h>
#include "AuthenticationProviderImpl.h"

using namespace std;

ProvideImplementationForDependency<AuthenticationProviderImpl, AuthenticationProvider> authenticationProviderDependency;

AuthenticationProviderImpl::AuthenticationProviderImpl ( BitBucketConfiguration *configuration ) :
    bitBucketConfiguration ( DependentOn<BitBucketConfiguration> ( configuration ) ) {}

cpr::Authentication AuthenticationProviderImpl::getAuthentication()
{
    Credentials savedCredentials = bitBucketConfiguration->getCredentials();
    return cpr::Authentication ( username.c_str(), password.c_str() );
}
