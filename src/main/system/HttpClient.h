#ifndef MUSTARD_HTTPCLIENT_H
#define MUSTARD_HTTPCLIENT_H


#include "../authentication/AuthenticationProvider.h"
#include "HttpResponse.h"

using namespace std;

class HttpClient
{
public:
    HttpClient ( AuthenticationProvider *authenticationProvider = nullptr );
    virtual HttpResponse get ( const string &url );
    virtual HttpResponse post ( const string &url, const string &body );
    virtual ~HttpClient() {}
private:
    AuthenticationProvider *authenticationProvider;
};


#endif //MUSTARD_HTTPCLIENT_H
