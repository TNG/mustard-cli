#ifndef MUSTARD_BITBUCKETCLIENTEXCEPTION_H
#define MUSTARD_BITBUCKETCLIENTEXCEPTION_H


#include <exception>
#include <string>
#include "../error/MustardException.h"

using namespace std;

class BitBucketClientException : public MustardException
{
public:
    BitBucketClientException ( const char *reason ) : MustardException ( reason ) {}
    BitBucketClientException ( const string &reason ) : MustardException ( reason ) {}
};

#endif //MUSTARD_BITBUCKETCLIENTEXCEPTION_H
