#include <Depend.h>
#include "StopReviewWorkflow.h"
#include "../comments/CommentConsumer.h"
#include "../git/GitClient.h"
#include "../bitbucket/PullRequest.h"
#include "../bitbucket/BitBucketClient.h"
#include "../bitbucket/BitBucketCommentUploader.h"

StopReviewWorkflow::StopReviewWorkflow ( CommentExtractor *commentExtractor, GitClient *gitClient,
        BitBucketClient *bitBucketClient ) :
    commentExtractor ( DependentOn<CommentExtractor> ( commentExtractor ) ),
    gitClient ( DependentOn<GitClient> ( gitClient ) ),
    bitBucketClient ( DependentOn<BitBucketClient> ( bitBucketClient ) ) {}

int StopReviewWorkflow::run ( int argc, const char **argv )
{
    const Commitish originFeatureHead = gitClient->getFeatureBranchOnOrigin();
    gitClient->reset ( originFeatureHead );
    const Comments comments = commentExtractor->extract();
    if ( comments.isEmpty() ) {
        printf ( "You did not make any comments.\n" );
        return -1;
    }
    printCommentSummary ( comments );

    const PullRequest pullRequest = bitBucketClient->getPullRequestFor ( originFeatureHead );

    BitBucketCommentUploader commentUploader ( pullRequest );
    comments.accept ( commentUploader );

    printf ( "Visit Bitbucket pull-request under:  %s\n", pullRequest.url.c_str() );
    return 0;
}

void StopReviewWorkflow::printCommentSummary ( const Comments &comments ) const
{
    class : public CommentConsumer
    {
        void consume ( const string &file, unsigned int line, const string &comment ) {
            if ( lastFile != file ) {
                lastFile = file;
                printf ( "---- %s ----\n", file.c_str() );
            }
            printf ( "%6.u\t%s\n", line, comment.c_str() );
        }

    private:
        string lastFile = "";
    } consumer;
    comments.accept ( consumer );
}
