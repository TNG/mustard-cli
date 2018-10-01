#include <Depend.h>
#include <Provide.h>
#include "HttpClient.h"

ProvideDependency<HttpClient> httpClientDependency;

HttpClient::HttpClient ( AuthenticationProvider *authenticationProvider ) :
    authenticationProvider ( DependentOn<AuthenticationProvider> ( authenticationProvider ) )
{

}

HttpResponse HttpClient::get ( const string &url )
{
    auto authentication = authenticationProvider->getAuthentication();
    auto response = cpr::Get (
                        cpr::Url ( url ),
    cpr::Header{{"accept", "application/json"}},
    authentication,
    cpr::VerifySsl{false} );
    return {
        response.text,
        response.status_code,
        response.error.code == cpr::ErrorCode::OK
    };
}

HttpResponse HttpClient::post ( const string &url, const string &body )
{
    auto authentication = authenticationProvider->getAuthentication();
    auto response = cpr::Post (
                        cpr::Url ( url ),
    cpr::Header{{"accept", "application/json"}},
    authentication,
    cpr::VerifySsl{false} );
    return {
        response.text,
        response.status_code,
        response.error.code == cpr::ErrorCode::OK
    };
}
