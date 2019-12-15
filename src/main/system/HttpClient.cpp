#include <Depend.h>
#include <Provide.h>
#include <iostream>
#include "HttpClient.h"
#include "../error/MustardException.h"

ProvideDependency<HttpClient> httpClientDependency;

HttpClient::HttpClient ( AuthenticationProvider *authenticationProvider,
                         CommandlineConfiguration *commandlineConfiguration ) :
    authenticationProvider ( DependentOn<AuthenticationProvider> ( authenticationProvider ) ),
    commandlineConfiguration ( DependentOn<CommandlineConfiguration> ( commandlineConfiguration ) )
{

}

HttpResponse HttpClient::get ( const string &url )
{
    log ( "GET", url, "- none -" );
    auto authentication = authenticationProvider->getAuthentication();
    auto response = cpr::Get (
                        cpr::Url ( url ),
    cpr::Header{{"accept", "application/json"}},
    authentication,
    cpr::VerifySsl{false} );

    HttpResponse res =  {
        response.text,
        response.status_code,
        response.status_code >= 200 && response.status_code <= 202
    };

    if ( !res.successful ) {
        stringstream ss;
        ss << "HTTP Communication error: " << response.error.message;
        throw MustardException ( ss.str() );
    }
    log ( res );
    return ( res );
}

HttpResponse HttpClient::post ( const string &url, const string &body )
{
    log ( "POST", url, body );
    auto authentication = authenticationProvider->getAuthentication();
    auto response = cpr::Post (
                        cpr::Url ( url ),
    cpr::Header{{"Content-Type",      "application/json"},
        {"accept",            "application/json"},
        {"X-Atlassian-Token", "no-check"}},
    authentication,
    cpr::Body ( body ),
    cpr::VerifySsl{false} );
    HttpResponse res = {
        response.text,
        response.status_code,
        response.error.code == cpr::ErrorCode::OK
    };
    log ( res );
    return res;
}

HttpResponse HttpClient::put ( const string &url, const string &body )
{
    log ( "PUT", url, body );
    auto authentication = authenticationProvider->getAuthentication();
    auto response = cpr::Put (
                        cpr::Url ( url ),
    cpr::Header{{"Content-Type",      "application/json"},
        {"accept",            "application/json"},
        {"X-Atlassian-Token", "no-check"}},
    authentication,
    cpr::Body ( body ),
    cpr::VerifySsl{false} );
    HttpResponse res = {
        response.text,
        response.status_code,
        response.error.code == cpr::ErrorCode::OK
    };
    log ( res );
    return res;
}

void HttpClient::log ( const string &method, const string &url, const string &body )
{
    if ( !commandlineConfiguration->isDebug() ) {
        return;
    }
    std::cerr << method << " -> " << url << endl
              << "Request Body: " << body << endl;

}

void HttpClient::log ( HttpResponse &response )
{
    if ( !commandlineConfiguration->isDebug() ) {
        return;
    }
    std::cerr << "Response:" << endl
              << "Status: " << response.httpStatus << endl
              << "Body: " << response.body << endl << endl;
}
