#include <cpr/cpr.h>
#include <Depend.h>
#include "BitBucketClientImpl.h"

BitBucketClientImpl::BitBucketClientImpl ( AuthenticationProvider *authenticationProvider ) :
    authenticationProvider ( DependentOn<AuthenticationProvider> ( authenticationProvider ) )
{
}

Commitish BitBucketClientImpl::getPullRequestTargetFor ( const Commitish &commit )
{
    auto authentication = authenticationProvider->getAuthentication();
    auto response = cpr::Get (
                        cpr::Url ( "https://bitbucket.int.tngtech.com/users/imgrundm/repos/poormansdi/pull-requests" ),
    cpr::Header{{"accept", "application/json"}},
    authentication,
    cpr::VerifySsl{false} );
    printf ( "%d - %d - %s\n", response.error, response.status_code, response.text.c_str() );
    return Commitish();
}
