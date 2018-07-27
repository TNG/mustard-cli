#ifndef MUSTARD_COMMANDRUNNER_H
#define MUSTARD_COMMANDRUNNER_H

class CommandRunner
{
public:
    virtual CommandResult run ( const string &command ) = 0;
    virtual ~CommandRunner() {}
};

#endif //MUSTARD_COMMANDRUNNER_H
