#ifndef MUSTARD_GITCLIENT_H
#define MUSTARD_GITCLIENT_H


#include "Commitish.h"

class GitClient
{
public:
    virtual Commitish getHeadCommit() = 0;
};

#endif //MUSTARD_GITCLIENT_H
