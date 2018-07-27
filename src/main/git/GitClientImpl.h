#ifndef MUSTARD_GITCLIENTIMPL_H
#define MUSTARD_GITCLIENTIMPL_H


#include <Provide.h>
#include "../system/CommandRunner.h"
#include "GitClient.h"

class GitClientImpl : public GitClient
{
public:
    Commitish getHeadCommit() override;

    GitClientImpl ( CommandRunner *commandRunner = nullptr );
private:
    CommandRunner *commandRunner;
};


#endif //MUSTARD_GITCLIENTIMPL_H
