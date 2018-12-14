#ifndef MUSTARD_CONTEXTDEFINITIONSTATE_H
#define MUSTARD_CONTEXTDEFINITIONSTATE_H


#include "../CommentState.h"

class ContextDefinitionState : public CommentState
{
public:
    ContextDefinitionState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier );

    CommentState *traverse ( LineClassifier::LineType lineType ) override;
};


#endif //MUSTARD_CONTEXTDEFINITIONSTATE_H
