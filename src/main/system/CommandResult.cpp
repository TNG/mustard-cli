#include "CommandResult.h"

CommandResult::CommandResult ( int returnCode, const string &output ) :
    returnCode ( returnCode ), output ( output )
{}

int CommandResult::getReturnCode() const
{
    return returnCode;
}

const string &CommandResult::getOutput() const
{
    return output;
}
