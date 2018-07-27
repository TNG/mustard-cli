#ifndef MUSTARD_COMMANDRUNNER_H
#define MUSTARD_COMMANDRUNNER_H

#include "CommandResult.h"

class CommandRunner
{
public:
    virtual CommandResult run ( const string &command ) = 0;
    virtual ~CommandRunner() {}
};

#endif //MUSTARD_COMMANDRUNNER_H
