#include "MultiLineCommentState.h"
#include "FileDiffState.h"
#include "../RegexMatcher.h"

MultiLineCommentState::MultiLineCommentState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier )
    : CommentState ( commentStateListener, lineClassifier ) {}

void MultiLineCommentState::consume ( const string &line )
{
    if ( !comment.empty() ) {
        comment += "\n";
    }
    static RegexMatcher commentOnly ( R"(^\+\s*(?:/\*~|\*)?\s*((?:[^\*]*|\*[^/])*)(?:\*/)?$)" );
    comment += commentOnly.getSingleCaptureIn ( line );
}

CommentState *MultiLineCommentState::traverse ( LineClassifier::LineType lineType )
{
    if ( lastLine ) {
        listener->newComment ( "", comment );
        return ( new FileDiffState ( listener, lineClassifier ) )->traverse ( lineType );
    }
    switch ( lineType ) {
    case LineClassifier::MULTILINECOMMENT_END:
        lastLine = true;
    default:
        return this;
    }
}
