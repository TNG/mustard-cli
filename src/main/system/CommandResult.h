#ifndef MUSTARD_COMMANDRESULT_H
#define MUSTARD_COMMANDRESULT_H

#include <string>

using namespace std;

class CommandResult
{
public:
    CommandResult ( int returnCode, const string &output );
    int getReturnCode() const;
    const string &getOutput() const;
    const string getOutputStripNewline() const;
private:
    const int returnCode;
    const string output;
};


#endif //MUSTARD_COMMANDRESULT_H
