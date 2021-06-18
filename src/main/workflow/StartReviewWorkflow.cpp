#include <iostream>
#include <Depend.h>
#include "StartReviewWorkflow.h"
#include "../bitbucket/PullRequestFormatter.h"
#include "../comments/CommentAppender.h"
#include "../error/MustardException.h"

using namespace std;

StartReviewWorkflow::StartReviewWorkflow ( BitBucketClient *bitBucketClient, GitClient *gitClient ) :
    bitBucketClient ( DependentOn<BitBucketClient> ( bitBucketClient ) ),
    gitClient ( DependentOn<GitClient> ( gitClient ) ) {}

int StartReviewWorkflow::run ( int argc, const char **argv )
{
    Commitish headCommit = gitClient->getFeatureBranchOnOrigin();
    PullRequest pullRequest = bitBucketClient->getPullRequestFor ( headCommit );

    cout << PullRequestFormatter::format ( pullRequest );

    Commitish pullRequestTarget = bitBucketClient->getPullRequestTargetFor ( headCommit );
    gitClient->reset ( pullRequestTarget, true );
    if ( !gitClient->merge ( headCommit ) ) {
        printf ( "***You are reviewing a pull request with a merge-conflict***\n***Comments on files with merge conflict are not supported***\n" );
    }
    gitClient->reset ( pullRequestTarget );

    const Comments comments = bitBucketClient->getCommentsFor ( pullRequest );

    gitClient->changeToRootDir();

    CommentAppender commentAppender;
    comments.accept ( commentAppender );
    commentAppender.finish();

    printf ( "Successfully prepared working directory for review.\n" );
    return 0;
}

