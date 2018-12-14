#ifndef MUSTARD_FILEDEFINITIONCONSUMER_H
#define MUSTARD_FILEDEFINITIONCONSUMER_H


#include "../LineConsumer.h"

class FileDefinitionConsumer : public LineConsumer
{
public:
    FileDefinitionConsumer ( CommentStateListener *listener );

    void consume ( const string &line ) override;
private:
    CommentStateListener *listener;
};


#endif //MUSTARD_FILEDEFINITIONCONSUMER_H
