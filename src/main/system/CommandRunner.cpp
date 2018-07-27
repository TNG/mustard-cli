#include <iostream>
#include <sstream>
#include <cstdio>
#include <stdlib.h>

#include "CommandRunner.h"

using namespace std;

CommandResult CommandRunner::run ( const string &command )
{
    if ( command.find ( '"' ) != string::npos ) {
        throw exception();
    }
    const string shellCommmand = "/usr/bin/bash -c \"" + command + "\"";
    FILE *outputPipe = popen ( shellCommmand.c_str(), "r" );
    string outputString;
    char line [1024];
    while ( fgets ( line, 1024, outputPipe ) ) {
        outputString += line;
    }
    const int returnCode = pclose ( outputPipe );
    return CommandResult ( WEXITSTATUS ( returnCode ), outputString );
}
