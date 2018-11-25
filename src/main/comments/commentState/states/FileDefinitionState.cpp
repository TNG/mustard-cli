#include "FileDefinitionState.h"
#include "../RegexMatcher.h"
#include "ContextDefinitionState.h"

FileDefinitionState::FileDefinitionState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier )
    : CommentState ( commentStateListener, lineClassifier ) {}

void FileDefinitionState::consume ( const string &line )
{
    static RegexMatcher fileName ( R"(^\+\+\+ b/(.*)$)" );
    string extractedFileName = fileName.getSingleCaptureIn ( line );
    if ( !extractedFileName.empty() ) {
        listener->newFile ( extractedFileName );
        hasFoundAddFileDefinition = true;
    }
}

CommentState *FileDefinitionState::traverse ( LineClassifier::LineType lineType )
{
    if ( hasFoundAddFileDefinition && lineType == LineClassifier::LineType::CONTEXTDEFINITION ) {
        return new ContextDefinitionState ( listener, lineClassifier );
    }
    return CommentState::traverse ( lineType );
}
