#include "MultiLineCommentState.h"
#include "FileDiffState.h"
#include "../RegexMatcher.h"

MultiLineCommentState::MultiLineCommentState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier )
    : CommentState ( commentStateListener, lineClassifier ) {}

void MultiLineCommentState::consume ( const string &line )
{
    static regex foreignCommentRegex ( R"(^\+\s*/\*~.*~.*)" );
    if ( foreignComment ||  regex_match ( line, foreignCommentRegex ) ) {
        foreignComment = true;
        return;
    }
    if ( !comment.empty() ) {
        comment += "\n";
    }
    static RegexMatcher commentOnly ( R"(^\+\s*(?:/\*~|\*)?\s*((?:[^\*]*|\*[^/])*)(?:\*/)?$)" );
    comment += commentOnly.getSingleCaptureIn ( line );
}

CommentState *MultiLineCommentState::traverse ( LineClassifier::LineType lineType )
{
    if ( lastLine ) {
        if ( !foreignComment ) {
            listener->newComment ( "", comment );
        }
        return ( new FileDiffState ( listener, lineClassifier ) )->traverse ( lineType );
    }
    if ( lineType == LineClassifier::MULTILINECOMMENT_END ) {
        lastLine = true;
    }
    return this;
}
