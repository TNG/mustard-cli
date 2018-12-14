#ifndef MUSTARD_MULTILINECOMMENTSTATE_H
#define MUSTARD_MULTILINECOMMENTSTATE_H


#include "../CommentState.h"

class MultiLineCommentState : public CommentState
{
public:
    MultiLineCommentState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier );

    void consume ( const string &line ) override;

    CommentState *traverse ( LineClassifier::LineType lineType ) override;
private:
    string comment;
    bool lastLine = false;
    bool foreignComment = false;
};


#endif //MUSTARD_MULTILINECOMMENTSTATE_H
