#ifndef MUSTARD_STARTREVIEWWORKFLOW_H
#define MUSTARD_STARTREVIEWWORKFLOW_H


#include "../bitbucket/BitBucketClient.h"
#include "../git/GitClient.h"
#include "Workflow.h"

class StartReviewWorkflow : public Workflow
{
public:
    StartReviewWorkflow ( BitBucketClient *bitBucketClient = nullptr, GitClient *gitClient = nullptr );

    int run ( int argc, const char **argv ) override;

private:
    BitBucketClient *bitBucketClient;
    GitClient *gitClient;
};


#endif //MUSTARD_STARTREVIEWWORKFLOW_H
