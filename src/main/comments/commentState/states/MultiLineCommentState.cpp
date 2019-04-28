#include <iostream>
#include "MultiLineCommentState.h"
#include "FileDiffState.h"
#include "../RegexMatcher.h"
#include "../lineConsumers/MultiLineCommentConsumer.h"

MultiLineCommentState::MultiLineCommentState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier )
    : CommentState ( commentStateListener, consumer, lineClassifier ),
      consumer ( commentStateListener ) {}

shared_ptr<CommentState> MultiLineCommentState::traverse ( LineClassifier::LineType lineType )
{
    if ( lastLineEncountered ) {
        return ( make_shared<FileDiffState> ( listener, lineClassifier ) )->traverse ( lineType );
    }
    if ( lineType == LineClassifier::REPLY_START || lineType == LineClassifier::COMMENT_ID_START ) {
        return  make_shared<MultiLineCommentState> ( listener, lineClassifier, &consumer );
    }
    if ( lineType == LineClassifier::REPLY_AND_END ) {
        return  ( make_shared<MultiLineCommentState> ( listener, lineClassifier, &consumer ) )
                ->traverse ( LineClassifier::MULTILINECOMMENT_END );
    }
    if ( lineType == LineClassifier::MULTILINECOMMENT_END ) {
        lastLineEncountered = true;
    }
    return shared_from_this();
}

MultiLineCommentState::MultiLineCommentState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier,
        MultiLineCommentConsumer *inReplyTo )
    : CommentState ( commentStateListener, consumer, lineClassifier ),
      consumer ( commentStateListener, inReplyTo ) {}
