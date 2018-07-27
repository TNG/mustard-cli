#include <algorithm>
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

const string CommandResult::getOutputStripNewline() const
{
    string str = output;
    str.erase ( std::remove ( str.begin(), str.end(), '\n' ), str.end() );
    return str;
}
