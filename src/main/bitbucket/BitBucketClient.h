#ifndef MUSTARD_BITBUCKETCLIENT_H
#define MUSTARD_BITBUCKETCLIENT_H

#include <vector>
#include "../git/Commitish.h"
#include "model/PullRequest.h"
#include "../comments/Comments.h"

using namespace std;

class BitBucketClient
{
public:
    virtual Commitish getPullRequestTargetFor ( const Commitish &commit ) = 0;
    virtual PullRequest getPullRequestFor ( const Commitish &commit ) = 0;
    virtual vector<PullRequest> getPullRequests() = 0;
    virtual Comments getCommentsFor ( const PullRequest &pullRequest ) = 0;
    virtual void approve ( const PullRequest &pullRequest, ReviewStatus reviewStatus ) = 0;
};

#endif //MUSTARD_BITBUCKETCLIENT_H
