#ifndef MUSTARD_MULTILINECOMMENTREPLYSTATE_H
#define MUSTARD_MULTILINECOMMENTREPLYSTATE_H


#include "MultiLineCommentState.h"

class MultiLineCommentReplyState : public MultiLineCommentState
{
public:
    MultiLineCommentReplyState ( MultiLineCommentConsumer *inReplyTo, CommentStateListener *commentStateListener,
                                 LineClassifier *lineClassifier );
    shared_ptr<CommentState> traverse ( LineClassifier::LineType lineType ) override;
};


#endif //MUSTARD_MULTILINECOMMENTREPLYSTATE_H
