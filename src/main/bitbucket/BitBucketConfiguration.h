#ifndef MUSTARD_BITBUCKETCONFIGURATION_H
#define MUSTARD_BITBUCKETCONFIGURATION_H


#include "../git/GitClient.h"

class BitBucketConfiguration
{
public:
    BitBucketConfiguration ( GitClient *gitClient = nullptr );
    virtual const string getBitBucketEndpoint();

private:

    GitClient *gitClient;
};


#endif //MUSTARD_BITBUCKETCONFIGURATION_H
