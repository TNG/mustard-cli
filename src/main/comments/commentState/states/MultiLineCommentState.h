#ifndef MUSTARD_MULTILINECOMMENTSTATE_H
#define MUSTARD_MULTILINECOMMENTSTATE_H


#include "../CommentState.h"

class MultiLineCommentState : public CommentState
{
public:
    MultiLineCommentState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier );
    CommentState *traverse ( LineClassifier::LineType lineType ) override;
private:
    bool lastLineEncountered = false;
};


#endif //MUSTARD_MULTILINECOMMENTSTATE_H
