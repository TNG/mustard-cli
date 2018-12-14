#ifndef MUSTARD_FILEDIFFLINECONSUMER_H
#define MUSTARD_FILEDIFFLINECONSUMER_H


#include "../CommentStateListener.h"
#include "../LineConsumer.h"

class FileDiffLineConsumer : public LineConsumer
{
public:
    FileDiffLineConsumer ( CommentStateListener *listener );
    void consume ( const string &line ) override;
private:
    CommentStateListener *listener;

    void consumeInlineComment ( const string &line ) const;

    void consumeLineNumberIncrease ( const string &line );
};


#endif //MUSTARD_FILEDIFFLINECONSUMER_H
