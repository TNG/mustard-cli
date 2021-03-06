#ifndef MUSTARD_FILEDEFINITIONSTATE_H
#define MUSTARD_FILEDEFINITIONSTATE_H

#include "../CommentState.h"
#include "../lineConsumers/FileDefinitionConsumer.h"

class FileDefinitionState : public CommentState
{
public:
    FileDefinitionState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier );

    shared_ptr<CommentState> traverse ( LineClassifier::LineType lineType ) override;

private:
    CommentStateListener *listener;
    FileDefinitionConsumer consumer;
};


#endif //MUSTARD_FILEDEFINITIONSTATE_H
