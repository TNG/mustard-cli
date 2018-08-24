#ifndef MUSTARD_HTTPCLIENTFORTEST_H
#define MUSTARD_HTTPCLIENTFORTEST_H
#include <gmock/gmock.h>
#include "../../main/system/HttpClient.h"
using namespace std;
using namespace testing;

class HttpClientForTest : public HttpClient
{
public:
    MOCK_METHOD1 ( get, HttpResponse ( const string & ) );
};

#endif //MUSTARD_HTTPCLIENTFORTEST_H
