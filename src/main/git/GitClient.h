#ifndef MUSTARD_GITCLIENT_H
#define MUSTARD_GITCLIENT_H
#include <gmock/gmock.h>

#include "Commitish.h"

class GitClient
{
public:
    virtual Commitish getHeadCommit() = 0;
    virtual bool workingDirectoryIsClean() = 0;
    virtual Commitish getMergeBase ( const Commitish &fromBranch, const Commitish &toBranch ) = 0;
    virtual Commitish getFeatureBranchOnOrigin() = 0;
    virtual void reset ( const Commitish &resetTo ) = 0;
    virtual string getConfigValue ( const string &name ) = 0;
    virtual string getDiff() = 0;
    virtual ~GitClient() {}
};

#endif //MUSTARD_GITCLIENT_H
