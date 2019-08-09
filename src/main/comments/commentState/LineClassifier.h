#ifndef MUSTARD_LINECLASSIFIER_H
#define MUSTARD_LINECLASSIFIER_H

#include <string>
#include <regex>

using namespace std;

class LineClassifier
{
public:
    enum LineType {
        UNKNOWN,
        FILEDEFINITION,
        CONTEXTDEFINITION,
        C_STYLE_SINGLE_LINE_COMMENT,
        MULTILINECOMMENT_START,
        REPLY_AND_END,
        REPLY_START,
        COMMENT_ID_START,
        MULTILINECOMMENT_END,
        ADDFILE,
        ADDLINE,
        DELLINE,
        ENDOFFILE
    };

    LineType classifyLine ( const string &line );

private:
    static map<LineType, std::vector< regex>> lineTypeRegexes;

    bool regexMatches ( const string &line, vector<regex> &regexList ) const ;
};


#endif //MUSTARD_LINECLASSIFIER_H
