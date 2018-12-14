#ifndef MUSTARD_CONTEXTDEFINITIONCONSUMER_H
#define MUSTARD_CONTEXTDEFINITIONCONSUMER_H


#include "../LineConsumer.h"

class ContextDefinitionConsumer : public LineConsumer
{
public:
    ContextDefinitionConsumer ( CommentStateListener *commentStateListener ) :
        listener ( commentStateListener )
    {}

    void consume ( const string &line ) override;
private:
    CommentStateListener *listener;
};


#endif //MUSTARD_CONTEXTDEFINITIONCONSUMER_H
