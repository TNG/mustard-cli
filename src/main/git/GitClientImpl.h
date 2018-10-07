#ifndef MUSTARD_GITCLIENTIMPL_H
#define MUSTARD_GITCLIENTIMPL_H


#include <Provide.h>
#include "../system/CommandRunner.h"
#include "GitClient.h"

class GitClientImpl : public GitClient
{
public:
    Commitish getHeadCommit() override;
    bool workingDirectoryIsClean() override;
    Commitish getMergeBase ( const Commitish &fromBranch, const Commitish &toBranch ) override;
    Commitish getFeatureBranchOnOrigin() override;
    void reset ( const Commitish &resetTo ) override;
    string getConfigValue ( const string &name ) override ;
    void setConfigValue ( const string &name, const string &value ) override ;
    string getDiff() override;

    GitClientImpl ( CommandRunner *commandRunner = nullptr );
private:
    CommandRunner *commandRunner;
};


#endif //MUSTARD_GITCLIENTIMPL_H
