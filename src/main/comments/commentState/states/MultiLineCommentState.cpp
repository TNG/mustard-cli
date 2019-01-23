#include <iostream>
#include "MultiLineCommentState.h"
#include "FileDiffState.h"
#include "../RegexMatcher.h"
#include "../lineConsumers/MultiLineCommentConsumer.h"
#include "MultiLineCommentReplyState.h"

MultiLineCommentState::MultiLineCommentState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier )
    : CommentState ( commentStateListener, consumer, lineClassifier ),
      consumer ( commentStateListener ) {}

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

MultiLineCommentState::MultiLineCommentState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier,
        MultiLineCommentConsumer *inReplyTo )
    : CommentState ( commentStateListener, consumer, lineClassifier ),
      consumer ( commentStateListener ) {}
