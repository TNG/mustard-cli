#ifndef MUSTARD_INBOXWORKFLOW_H
#define MUSTARD_INBOXWORKFLOW_H


#include "Workflow.h"
#include "../bitbucket/BitBucketClient.h"
#include "../git/GitClient.h"

class InboxWorkflow : public Workflow
{
public:
    InboxWorkflow ( BitBucketClient *bitBucketClient = nullptr, GitClient *gitClient = nullptr ) ;
    int run ( int argc, const char **argv ) override;
private:
    BitBucketClient *bitbucketClient;
    GitClient *gitClient;

    static bool equalsCaseInsensitive ( const string &a, const string &b );
};


#endif //MUSTARD_INBOXWORKFLOW_H
