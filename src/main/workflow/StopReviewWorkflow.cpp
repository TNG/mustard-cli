#include <Depend.h>
#include "StopReviewWorkflow.h"
#include "../comments/CommentConsumer.h"
#include "../git/GitClient.h"
#include "../bitbucket/model/PullRequest.h"
#include "../bitbucket/BitBucketClient.h"
#include "../bitbucket/BitBucketCommentUploader.h"
#include "../system/UserConfirmation.h"

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
    } else {
        handleCommentUpload ( originFeatureHead, comments );
    }

    if ( UserConfirmation ( "Should I reset the feature branch and discard all of your review changes?" ).askUser() == YES ) {
        gitClient->reset ( originFeatureHead, true );
    }

    return 0;
}

void StopReviewWorkflow::handleCommentUpload ( const Commitish &originFeatureHead, const Comments &comments ) const
{
    printCommentSummary ( comments );

    if ( UserConfirmation ( "Do you want to upload these comments now?" ).askUser() == NO ) {
        cout << "will not upload comments" << endl;
        return;
    }

    const PullRequest pullRequest = bitBucketClient->getPullRequestFor ( originFeatureHead );

    printf ( "Posting comments...\n" );
    BitBucketCommentUploader commentUploader ( pullRequest );
    comments.accept ( commentUploader );
    printf ( "Uploaded %u of %u comments\n",
             commentUploader.getUploadedCommentNumber(),
             commentUploader.getSeenCommentNumber() );
    printf ( "Visit Bitbucket pull-request under:  %s\n", pullRequest.url.c_str() );
}

void StopReviewWorkflow::printCommentSummary ( const Comments &comments ) const
{
    class : public CommentConsumer
    {
        void consume ( const string &file, const LineComment &lineComment ) {
            if ( lastFile != file ) {
                lastFile = file;
                printf ( "---- %s ----\n", file.c_str() );
            }
            printf ( "%6.u\t%s\n", lineComment.getLine(), lineComment.getComment().c_str() );
        }

    private:
        string lastFile = "";
    } consumer;
    comments.accept ( consumer );
}
