#ifndef MUSTARD_HTTPCLIENT_H
#define MUSTARD_HTTPCLIENT_H


#include "../authentication/AuthenticationProvider.h"
#include "../workflow/CommandlineConfiguration.h"
#include "HttpResponse.h"

using namespace std;

class HttpClient
{
public:
    HttpClient ( AuthenticationProvider *authenticationProvider = nullptr,
                 CommandlineConfiguration *commandlineConfiguration = nullptr );
    virtual HttpResponse get ( const string &url );
    virtual HttpResponse post ( const string &url, const string &body );
    virtual HttpResponse put ( const string &url, const string &body );
    virtual ~HttpClient() {}
private:
    AuthenticationProvider *authenticationProvider;
    CommandlineConfiguration *commandlineConfiguration;
    void log ( const string &url, const string &body, const string &method );
    void log ( HttpResponse &response );

    HttpResponse logAndConvertToHttpResponse ( const cpr::Response &response );
};


#endif //MUSTARD_HTTPCLIENT_H
