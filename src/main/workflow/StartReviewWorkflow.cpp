#include <Depend.h>
#include "StartReviewWorkflow.h"
#include "../error/MustardException.h"

StartReviewWorkflow::StartReviewWorkflow ( BitBucketClient *bitBucketClient, GitClient *gitClient ) :
    bitBucketClient ( DependentOn<BitBucketClient> ( bitBucketClient ) ),
    gitClient ( DependentOn<GitClient> ( gitClient ) ) {}

int StartReviewWorkflow::run ( int argc, const char **argv )
{
    Commitish headCommit = gitClient->getHeadCommit();
    Commitish pullRequestTarget = bitBucketClient->getPullRequestTargetFor ( headCommit );
    Commitish baseCommit = gitClient->getMergeBase ( headCommit, pullRequestTarget );
    gitClient->reset ( baseCommit );
    printf ( "Successfully prepared working directory for review.\n" );
    return 0;
}

