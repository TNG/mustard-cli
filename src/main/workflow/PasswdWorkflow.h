#ifndef MUSTARD_PASSWDWORKFLOW_H
#define MUSTARD_PASSWDWORKFLOW_H


#include "Workflow.h"
#include "../bitbucket/BitBucketConfiguration.h"

class PasswdWorkflow : public Workflow
{
public:
    PasswdWorkflow ( BitBucketConfiguration *bitBucketConfiguration = nullptr );
    int run ( int argc, const char **argv ) override;
private:
    BitBucketConfiguration *configuration;
};


#endif //MUSTARD_PASSWDWORKFLOW_H
