#include <Provide.h>
#include "LineClassifier.h"

ProvideDependency<LineClassifier> lineClassifierDependency;

map <LineClassifier::LineType, regex> LineClassifier::lineTypeRegexes = {
    {FILEDEFINITION,    regex ( "^diff.*" ) },
    {CONTEXTDEFINITION, regex ( "^@@.*" ) },
    {ADDFILE,           regex ( "^\\+\\+\\+.*" ) },
    {MULTILINECOMMENT_START, regex ( R"(^\+\w*/\*~)" ) },
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
