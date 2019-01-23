#ifndef MUSTARD_FILEDIFFSTATE_H
#define MUSTARD_FILEDIFFSTATE_H


#include "../CommentState.h"
#include "../lineConsumers/FileDiffLineConsumer.h"

class FileDiffState : public CommentState
{
public:
    FileDiffState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier );

    shared_ptr<CommentState> traverse ( LineClassifier::LineType lineType ) override;
private:
    FileDiffLineConsumer consumer;
};


#endif //MUSTARD_FILEDIFFSTATE_H
