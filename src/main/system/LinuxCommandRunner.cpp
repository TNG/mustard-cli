#include <iostream>
#include <sstream>
#include <cstdio>
#include <stdlib.h>
#include <Provide.h>
#include <zconf.h>

#include "LinuxCommandRunner.h"

using namespace std;

ProvideImplementationForDependency<LinuxCommandRunner, CommandRunner> commandRunnerDependency;

CommandResult LinuxCommandRunner::run ( const string &command )
{
    if ( command.find ( '"' ) != string::npos ) {
        throw exception();
    }
    const string shellCommmand = "/usr/bin/bash -c \"" + command + "\"";
    FILE *outputPipe = popen ( shellCommmand.c_str(), "r" );
    string outputString;
    char line[1024];
    while ( fgets ( line, 1024, outputPipe ) ) {
        outputString += line;
    }
    const int returnCode = pclose ( outputPipe );
    return CommandResult ( WEXITSTATUS ( returnCode ), outputString );
}

void LinuxCommandRunner::cd ( const string &dir )
{
    chdir ( dir.c_str() );
}

string LinuxCommandRunner::pwd() const
{
    char pwd[10240];
    if ( !getcwd ( pwd, 10240 ) ) {
        throw exception();
    }
    return pwd;
}