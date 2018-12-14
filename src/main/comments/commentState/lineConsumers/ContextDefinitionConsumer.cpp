#include "ContextDefinitionConsumer.h"
#include "../RegexMatcher.h"

void ContextDefinitionConsumer::consume ( const string &line )
{
    static RegexMatcher lineNumberMatcher ( R"(^@@ -\d*(?:,\d*)? \+(\d*)(?:,\d*)? @@.*)" );
    string lineNumberString = lineNumberMatcher.getSingleCaptureIn ( line );
    if ( !lineNumberString.empty() ) {
        listener->setLine ( atoi ( lineNumberString.c_str() ) - 1 );
    }

}
