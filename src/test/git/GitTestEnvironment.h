#ifndef MUSTARD_GITTESTENVIRONMENT_H
#define MUSTARD_GITTESTENVIRONMENT_H


#include <iostream>
#include "../../main/system/CommandRunner.h"
#include "../../main/system/CommandResult.h"

class GitTestEnvironment
{
public:
    GitTestEnvironment ( CommandRunner *commandRunner = nullptr );
    ~GitTestEnvironment();
    CommandResult run ( const string &cmd );
    void createFileAndCommit ( const string &file );
private:
    CommandRunner *commandRunner;
    string pwd;
};
#endif //MUSTARD_GITTESTENVIRONMENT_H
