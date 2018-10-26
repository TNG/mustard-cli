#ifndef MUSTARD_BITBUCKETCLIENT_H
#define MUSTARD_BITBUCKETCLIENT_H

#include "../git/Commitish.h"
#include "model/PullRequest.h"
#include "../comments/Comments.h"

class BitBucketClient
{
public:
    virtual Commitish getPullRequestTargetFor ( const Commitish &commit ) = 0;
    virtual PullRequest getPullRequestFor ( const Commitish &commit ) = 0;
    virtual Comments getCommentsFor ( const PullRequest &pullRequest ) = 0;
};

#endif //MUSTARD_BITBUCKETCLIENT_H
