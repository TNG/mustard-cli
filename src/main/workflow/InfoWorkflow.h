#ifndef MUSTARD_INFOWORKFLOW_H
#define MUSTARD_INFOWORKFLOW_H

#include <iostream>
#include "Workflow.h"
#include "../bitbucket/BitBucketClient.h"
#include "../git/GitClient.h"

using namespace std;

class InfoWorkflow : public Workflow
{
public:
    InfoWorkflow ( BitBucketClient *bitBucketClient = nullptr, GitClient *gitClient = nullptr );
    int run ( int argc, const char **argv ) override;

private:
    BitBucketClient *bitBucketClient;
    GitClient *gitClient;

};


#endif //MUSTARD_INFOWORKFLOW_H
