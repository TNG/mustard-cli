#ifndef MUSTARD_MULTILINECOMMENTSTATE_H
#define MUSTARD_MULTILINECOMMENTSTATE_H


#include "../CommentState.h"
#include "../lineConsumers/MultiLineCommentConsumer.h"

class MultiLineCommentState : public CommentState
{
public:
    MultiLineCommentState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier );
    virtual shared_ptr<CommentState> traverse ( LineClassifier::LineType lineType ) override;

    MultiLineCommentState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier,
                            MultiLineCommentConsumer *inReplyTo );

protected:
    bool lastLineEncountered = false;
    MultiLineCommentConsumer consumer;
};


#endif //MUSTARD_MULTILINECOMMENTSTATE_H
