#ifndef MUSTARD_BITBUCKETCLIENTEXCEPTION_H
#define MUSTARD_BITBUCKETCLIENTEXCEPTION_H


#include <exception>
#include <string>

using namespace std;

class BitBucketClientException : public exception
{
public:
    BitBucketClientException ( const char *what ) : reason ( what ) {}
    const char *what() const noexcept {
        return reason.c_str();
    }
    ~BitBucketClientException() noexcept {}
private:
    const string reason;
};

#endif //MUSTARD_BITBUCKETCLIENTEXCEPTION_H
