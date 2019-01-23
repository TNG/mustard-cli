#include "MultiLineCommentReplyState.h"

MultiLineCommentReplyState::MultiLineCommentReplyState ( MultiLineCommentConsumer *inReplyTo,
        CommentStateListener *commentStateListener,
        LineClassifier *lineClassifier ) :
    MultiLineCommentState ( commentStateListener, lineClassifier, inReplyTo ) {}

shared_ptr<CommentState> MultiLineCommentReplyState::traverse ( LineClassifier::LineType lineType )
{
    if ( lineType == LineClassifier::REPLY_START ) {
        return shared_from_this();
    }
    if ( lineType == LineClassifier::COMMENT_ID_START ) {
        return make_shared<MultiLineCommentState> ( listener, lineClassifier );
    }
    return MultiLineCommentState::traverse ( lineType );
}


