#include "ContextDefinitionState.h"
#include "../RegexMatcher.h"
#include "FileDiffState.h"

ContextDefinitionState::ContextDefinitionState ( CommentStateListener *commentStateListener,
        LineClassifier *lineClassifier ) : CommentState ( commentStateListener,
                    lineClassifier ) {}

void ContextDefinitionState::consume ( const string &line )
{
    static RegexMatcher lineNumberMatcher ( R"(^@@ -\d* \+(\d*) @@.*)" );
    string lineNumberString = lineNumberMatcher.getSingleCaptureIn ( line );
    if ( !lineNumberString.empty() ) {
        listener->setLine ( atoi ( lineNumberString.c_str() ) - 1 );
    }
}

CommentState *ContextDefinitionState::traverse ( LineClassifier::LineType lineType )
{
    return new FileDiffState ( listener, lineClassifier );
}


