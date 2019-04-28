#include <Depend.h>
#include <regex>
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

    if ( UserConfirmation ( "Should I reset the feature branch and discard all of your review changes?" ).askUser() ==
            YES ) {
        gitClient->reset ( originFeatureHead, true );
    }

    UserChoice<ReviewStatus> reviewStatusChoice (
    "Should I [a]pprove the pullrequest, does it [n]eed work, or just [q]uit", {
        {'a', APPROVED},
        {'q', UNAPPROVED},
        {'n', NEEDS_WORK},
    } );

    const ReviewStatus userChoice = reviewStatusChoice.askUser();
    switch ( userChoice ) {
    case UNAPPROVED:
        break;
    default:
        PullRequest pullRequest = bitBucketClient->getPullRequestFor ( originFeatureHead );
        bitBucketClient->approve ( pullRequest, userChoice );
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
            lineComment.forEachReply ( [this, &file] ( const LineComment & lineComment ) {
                consume ( file, lineComment );
            } );
            if ( !lineComment.getAuthor().empty() ) {
                return;
            }
            if ( lastFile != file ) {
                lastFile = file;
                printf ( "---- %s ----\n", file.c_str() );
            }
            static regex newline ( "\n" );
            const string commentIndented = regex_replace ( lineComment.getComment(), newline, "\n\t" );
            printf ( "%6.u\t%s\n", lineComment.getLine(), commentIndented.c_str() );
        }

    private:
        string lastFile = "";
    } consumer;
    comments.accept ( consumer );
}
