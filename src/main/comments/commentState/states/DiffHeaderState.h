#ifndef MUSTARD_DIFFHEADERSTATE_H
#define MUSTARD_DIFFHEADERSTATE_H


#include "../CommentState.h"

class DiffHeaderState : public CommentState
{
public:
    DiffHeaderState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier = nullptr );

    CommentState *traverse ( LineClassifier::LineType lineType ) override;
};


#endif //MUSTARD_DIFFHEADERSTATE_H
