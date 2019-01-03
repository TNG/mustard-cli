#include "MultiLineCommentState.h"
#include "FileDiffState.h"
#include "../RegexMatcher.h"
#include "../lineConsumers/MultiLineCommentConsumer.h"

MultiLineCommentState::MultiLineCommentState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier )
    : CommentState ( commentStateListener, new MultiLineCommentConsumer ( commentStateListener ), lineClassifier ) {}

shared_ptr<CommentState> MultiLineCommentState::traverse ( LineClassifier::LineType lineType )
{
    if ( lastLineEncountered ) {
        return ( make_shared<FileDiffState> ( listener, lineClassifier ) )->traverse ( lineType );
    }
    if ( lineType == LineClassifier::MULTILINECOMMENT_END ) {
        lastLineEncountered = true;
    }
    return shared_from_this();
}