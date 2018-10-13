#ifndef MUSTARD_AUTOCONFWORKFLOW_H
#define MUSTARD_AUTOCONFWORKFLOW_H


#include "../bitbucket/BitBucketConfigGuesser.h"
#include "Workflow.h"

class AutoconfWorkflow : public Workflow
{
public:
    AutoconfWorkflow ( BitBucketConfigGuesser *guesser = nullptr, GitClient *gitClient = nullptr );
private:
    BitBucketConfigGuesser *guesser;
    GitClient *gitClient;
public:
    int run ( int argc, const char **argv ) override;

    void suggestIfNotConfigured ( const char *explanation, const char *key, const string &guessedValue );

    void suggest ( const char *explanation, const char *key, const string &guessedValue );
};


#endif //MUSTARD_AUTOCONFWORKFLOW_H
