#ifndef MUSTARD_LINECLASSIFIER_H
#define MUSTARD_LINECLASSIFIER_H

#include <string>
#include <regex>

using namespace std;

class LineClassifier
{
public:
    enum LineType { UNKNOWN, FILEDEFINITION, CONTEXTDEFINITION, ADDFILE, ADDLINE, DELLINE, MULTILINECOMMENT_START, MULTILINECOMMENT_END};
    bool isLineOfType ( LineType lineType, const string &line );

    LineType classifyLine ( const string &line );

private:
    static map<LineType, regex> lineTypeRegexes;
};


#endif //MUSTARD_LINECLASSIFIER_H
