#include <Depend.h>
#include <Provide.h>
#include <iostream>
#include "HttpClient.h"
#include "../error/MustardException.h"
#include "../git/GitClientException.h"

ProvideDependency<HttpClient> httpClientDependency;

HttpClient::HttpClient ( AuthenticationProvider *authenticationProvider,
                         CommandlineConfiguration *commandlineConfiguration,
                         GitClient *gitClient ) :
    authenticationProvider ( DependentOn<AuthenticationProvider> ( authenticationProvider ) ),
    commandlineConfiguration ( DependentOn<CommandlineConfiguration> ( commandlineConfiguration ) ),
    verifySsl ( shouldVerifySsl ( DependentOn<GitClient> ( gitClient ) ) )
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
    cpr::VerifySsl{verifySsl} );

    return ( ( logAndConvertToHttpResponse ( response ) ) );
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
    cpr::VerifySsl{verifySsl} );

    return logAndConvertToHttpResponse ( response );
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
    cpr::VerifySsl{verifySsl} );

    return logAndConvertToHttpResponse ( response );
}

void HttpClient::log ( const string &method, const string &url, const string &body )
{
    if ( !commandlineConfiguration->isDebug() ) {
        return;
    }
    std::cerr << method << " -> " << url << endl
              << "Request Body: " << body << endl;

}

HttpResponse HttpClient::logAndConvertToHttpResponse ( const cpr::Response &response )
{
    HttpResponse res =  {
        response.text,
        response.status_code,
        response.status_code >= 200 && response.status_code <= 202
    };

    log ( res );

    if ( !res.successful  && !response.error.message.empty() ) {
        stringstream ss;
        ss << "HTTP Communication error: " << response.error.message;
        throw MustardException ( ss.str() );
    }
    return res;
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

bool HttpClient::shouldVerifySsl ( GitClient *gitClient )
{
    try {
        auto connectInsecure = gitClient->getConfigValue ( "mustard.connectInsecure" );
        return ! ( "true" == connectInsecure );
    } catch ( GitClientException &e ) {
        return true;
    }
}
