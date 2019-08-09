#include <Provide.h>
#include "LineClassifier.h"

ProvideDependency<LineClassifier> lineClassifierDependency;

map <LineClassifier::LineType, vector< regex>> LineClassifier::lineTypeRegexes = {
    {FILEDEFINITION,                {regex ( "^diff" ) } },
    {CONTEXTDEFINITION,             {regex ( "^@@" ) } },
    {ADDFILE,                       {regex ( "^\\+\\+\\+" ) } },
    {C_STYLE_SINGLE_LINE_COMMENT,   {regex ( R"(^\+\s*/\*~)" ), regex ( R"(\*/)" ) } },
    {MULTILINECOMMENT_START,        {regex ( R"(^\+\s*/\*~)" ) } },
    {REPLY_AND_END,                 {regex ( R"(@reply)" ), regex ( R"(.*\*/)" ) } },
    {REPLY_START,                   {regex ( R"(@reply)" ) } },
    {COMMENT_ID_START,              {regex ( R"(^\+)" ), regex ( R"(@author\([^\)]+\))" ) } },
    {MULTILINECOMMENT_END,          {regex ( "^\\+" ), regex ( "\\*/" ) } },
    {ADDLINE,                       {regex ( "^\\+" ) } },
    {DELLINE,                       {regex ( "^\\-" ) } }
};


LineClassifier::LineType LineClassifier::classifyLine ( const string &line )
{
    for ( auto candidateType : lineTypeRegexes ) {
        if ( regexMatches ( line, candidateType.second ) ) {
            return candidateType.first;
        }
    }
    return UNKNOWN;
}

bool LineClassifier::regexMatches ( const string &line, vector<regex> &regexList ) const
{
    for ( const auto &regex : regexList ) {
        if ( !regex_search ( line, regex ) ) {
            return false;
        }
    }
    return true;
}
