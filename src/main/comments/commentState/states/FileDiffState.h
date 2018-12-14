#ifndef MUSTARD_FILEDIFFSTATE_H
#define MUSTARD_FILEDIFFSTATE_H


#include "../CommentState.h"

class FileDiffState : public CommentState
{
public:
    FileDiffState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier );

    CommentState *traverse ( LineClassifier::LineType lineType ) override;
};


#endif //MUSTARD_FILEDIFFSTATE_H
