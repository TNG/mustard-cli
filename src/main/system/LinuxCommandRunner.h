#ifndef MUSTARD_LINUXCOMMANDRUNNER_H
#define MUSTARD_LINUXCOMMANDRUNNER_H

#include <string>
#include "CommandResult.h"
#include "CommandRunner.h"

using namespace std;

class LinuxCommandRunner: public CommandRunner
{
public:
    virtual CommandResult run ( const string &command );
    virtual ~LinuxCommandRunner() {}

};


#endif //MUSTARD_LINUXCOMMANDRUNNER_H
