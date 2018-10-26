#ifndef MUSTARD_GITTESTENVIRONMENT_H
#define MUSTARD_GITTESTENVIRONMENT_H


#include <iostream>
#include "../../main/system/CommandRunner.h"
#include "../../main/system/CommandResult.h"

class GitTestEnvironment
{
public:
    GitTestEnvironment ( const string &baseDir = "/tmp/mustardTestEnv", CommandRunner *commandRunner = nullptr );
    ~GitTestEnvironment();
    CommandResult run ( const string &cmd );
    void createFileAndCommit ( const string &file );
private:
    CommandRunner *commandRunner;
    string pwd;
    string baseDir;
};
#endif //MUSTARD_GITTESTENVIRONMENT_H
