#include "FileDiffState.h"
#include "ContextDefinitionState.h"
#include "FileDefinitionState.h"
#include "../RegexMatcher.h"
#include "MultiLineCommentState.h"

FileDiffState::FileDiffState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier ) : CommentState (
        commentStateListener, lineClassifier ) {}

void FileDiffState::consume ( const string &line )
{
    static RegexMatcher commentMatcher ( R"(^\+.*//([^~]*)$)" );
    const string matchedComment = commentMatcher.getSingleCaptureIn ( line );
    if ( !matchedComment.empty() ) {
        listener->newComment ( "", matchedComment );
    }
}

CommentState *FileDiffState::traverse ( LineClassifier::LineType lineType )
{
    switch ( lineType ) {
    case LineClassifier::DELLINE:
        return this;
    case LineClassifier::CONTEXTDEFINITION:
        return new ContextDefinitionState ( listener, lineClassifier );
    case LineClassifier::FILEDEFINITION:
        return new FileDefinitionState ( listener, lineClassifier );
    case LineClassifier::MULTILINECOMMENT_START:
        return new MultiLineCommentState ( listener, lineClassifier );
    default:
        listener->newLine();
        return this;
    }
}
