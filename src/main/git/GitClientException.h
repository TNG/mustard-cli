#ifndef MUSTARD_GITCLIENTEXCEPTION_H
#define MUSTARD_GITCLIENTEXCEPTION_H

#include <exception>
#include <string>
#include "../error/MustardException.h"

using namespace std;

class GitClientException : public MustardException
{
public:
    GitClientException ( const char *what ) : MustardException ( what ) {}
};


#endif //MUSTARD_GITCLIENTEXCEPTION_H
