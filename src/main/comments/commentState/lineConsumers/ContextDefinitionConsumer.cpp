#include "ContextDefinitionConsumer.h"
#include "../RegexMatcher.h"

void ContextDefinitionConsumer::consume ( const string &line )
{
    static RegexMatcher lineNumberMatcher ( R"(^@@ -\d*(?:,\d*)? \+(\d*)(?:,\d*)? @@.*)" );
    const auto lineNumber = lineNumberMatcher.getSingleCaptureIn ( line );
    if ( lineNumber.has_value() ) {
        listener->setLine ( atoi ( lineNumber.value().c_str() ) - 1 );
    }

}
