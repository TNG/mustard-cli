#ifndef MUSTARD_BITBUCKETCLIENT_H
#define MUSTARD_BITBUCKETCLIENT_H

#include <pistache/client.h>
#include "../git/Commitish.h"

class BitBucketClient
{
    virtual Commitish getPullRequestTargetFor ( const Commitish &commit ) = 0;
};

#endif //MUSTARD_BITBUCKETCLIENT_H
