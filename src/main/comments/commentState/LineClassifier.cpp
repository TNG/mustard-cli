#include <Provide.h>
#include "LineClassifier.h"

ProvideDependency<LineClassifier> lineClassifierDependency;

map <LineClassifier::LineType, regex> LineClassifier::lineTypeRegexes = {
    {FILEDEFINITION,    regex ( "^diff.*" ) },
    {CONTEXTDEFINITION, regex ( "^@@.*" ) },
    {ADDFILE,           regex ( "^\\+\\+\\+.*" ) },
    {C_STYLE_SINGLE_LINE_COMMENT, regex ( R"(^\+\s*/\*~.*\\*/)" ) },
    {MULTILINECOMMENT_START, regex ( R"(^\+\s*/\*~.*)" ) },
    {REPLY_AND_END, regex ( R"(.*@reply.*\*/.*)" ) },
    {REPLY_START, regex ( R"(.*@reply.*)" ) },
    {COMMENT_ID_START, regex ( R"(^\+\s*\*\s*~.*@\d*~.*)" ) },
    {MULTILINECOMMENT_END, regex ( "^\\+.*\\*/" ) },
    {ADDLINE,           regex ( "^\\+.*" ) },
    {DELLINE,           regex ( "^\\-.*" ) }
};


bool LineClassifier::isLineOfType ( LineClassifier::LineType lineType, const string &line )
{
    return regex_match ( line, lineTypeRegexes[lineType] );
}

LineClassifier::LineType LineClassifier::classifyLine ( const string &line )
{
    for ( auto candidateType : lineTypeRegexes ) {
        if ( regex_match ( line, candidateType.second ) ) {
            return candidateType.first;
        }
    }
    return UNKNOWN;
}
