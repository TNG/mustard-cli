#ifndef MUSTARD_COMMANDRUNNER_H
#define MUSTARD_COMMANDRUNNER_H

#include <string>
#include "CommandResult.h"

using namespace std;

class CommandRunner
{
public:
    CommandResult run ( const string &command );

};


#endif //MUSTARD_COMMANDRUNNER_H
