#ifndef MUSTARD_HTTPRESPONSE_H
#define MUSTARD_HTTPRESPONSE_H

#include <string>

using namespace std;

struct HttpResponse {
    string body;
    long httpStatus;
    bool successful;
};

#endif //MUSTARD_HTTPRESPONSE_H
