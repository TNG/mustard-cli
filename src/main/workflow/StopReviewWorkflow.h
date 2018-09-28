#ifndef MUSTARD_STOPREVIEWWORKFLOW_H
#define MUSTARD_STOPREVIEWWORKFLOW_H


#include "Workflow.h"
#include "../comments/CommenExtractor.h"
#include "../git/GitClient.h"
#include "../bitbucket/BitBucketClient.h"

class StopReviewWorkflow : public Workflow
{
public:
    StopReviewWorkflow ( CommentExtractor *commentExtractor = nullptr,
                         GitClient *gitClient = nullptr,
                         BitBucketClient *bitBucketClient = nullptr );

    int run ( int argc, const char **argv ) override;

private:
    CommentExtractor *commentExtractor;
    GitClient *gitClient;
    BitBucketClient *bitBucketClient;

    void printCommentSummary ( const Comments &comments ) const;
};


#endif //MUSTARD_STOPREVIEWWORKFLOW_H
