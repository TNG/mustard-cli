#ifndef MUSTARD_CONTEXTDEFINITIONSTATE_H
#define MUSTARD_CONTEXTDEFINITIONSTATE_H


#include "../CommentState.h"

class ContextDefinitionState : public CommentState
{
public:
    ContextDefinitionState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier );

    CommentState *traverse ( LineClassifier::LineType lineType ) override;

    void consume ( const string &line ) override;
};


#endif //MUSTARD_CONTEXTDEFINITIONSTATE_H
