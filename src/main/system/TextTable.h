#ifndef MUSTARD_TEXTTABLE_H
#define MUSTARD_TEXTTABLE_H

#include <vector>
#include <string>

using namespace std;

class TextTable
{
public:
    TextTable ( int columns );

    void addRow ( const vector<string> &row );
    string getTable();
private:
    vector<vector<string>> rows;
    int columns;
    unsigned int lengthWithoutAnsiCommands ( const string &s );

};


#endif //MUSTARD_TEXTTABLE_H
