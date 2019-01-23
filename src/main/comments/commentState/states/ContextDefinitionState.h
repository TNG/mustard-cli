#ifndef MUSTARD_CONTEXTDEFINITIONSTATE_H
#define MUSTARD_CONTEXTDEFINITIONSTATE_H


#include "../CommentState.h"
#include "../lineConsumers/ContextDefinitionConsumer.h"

class ContextDefinitionState : public CommentState
{
public:
    ContextDefinitionState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier );

    shared_ptr<CommentState> traverse ( LineClassifier::LineType lineType ) override;
private:
    ContextDefinitionConsumer consumer;
};


#endif //MUSTARD_CONTEXTDEFINITIONSTATE_H
