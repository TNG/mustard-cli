#include <Depend.h>
#include "StartReviewWorkflow.h"
#include "../bitbucket/PullRequestFormatter.h"
#include "../comments/CommentAppender.h"

StartReviewWorkflow::StartReviewWorkflow ( BitBucketClient *bitBucketClient, GitClient *gitClient ) :
    bitBucketClient ( DependentOn<BitBucketClient> ( bitBucketClient ) ),
    gitClient ( DependentOn<GitClient> ( gitClient ) ) {}

int StartReviewWorkflow::run ( int argc, const char **argv )
{
    Commitish headCommit = gitClient->getFeatureBranchOnOrigin();
    PullRequest pullRequest = bitBucketClient->getPullRequestFor ( headCommit );

    cout << PullRequestFormatter::format ( pullRequest );

    Commitish pullRequestTarget = bitBucketClient->getPullRequestTargetFor ( headCommit );
    Commitish baseCommit = gitClient->getMergeBase ( headCommit, pullRequestTarget );
    gitClient->reset ( baseCommit );

    const Comments comments = bitBucketClient->getCommentsFor ( pullRequest );

    CommentAppender commentAppender;
    comments.accept ( commentAppender );
    commentAppender.finish();

    printf ( "Successfully prepared working directory for review.\n" );
    return 0;
}

