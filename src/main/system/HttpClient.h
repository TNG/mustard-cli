#ifndef MUSTARD_HTTPCLIENT_H
#define MUSTARD_HTTPCLIENT_H


#include "../authentication/AuthenticationProvider.h"
#include "HttpResponse.h"

using namespace std;

class HttpClient
{
public:
    HttpClient ( AuthenticationProvider *authenticationProvider = nullptr );
    HttpResponse get ( const string &url );
private:
    AuthenticationProvider *authenticationProvider;

};


#endif //MUSTARD_HTTPCLIENT_H
