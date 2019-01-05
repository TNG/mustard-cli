#ifndef MUSTARD_FILEDIFFLINECONSUMER_H
#define MUSTARD_FILEDIFFLINECONSUMER_H


#include "../CommentStateListener.h"
#include "../LineConsumer.h"
#include "../RegexMatcher.h"

class FileDiffLineConsumer : public LineConsumer
{
public:
    FileDiffLineConsumer ( CommentStateListener *listener );
    void consume ( const string &line ) override;
private:
    CommentStateListener *listener;

    void consumeLineNumberIncrease ( const string &line );

    void addCommentIfMatching ( const string &line, const RegexMatcher &commentMatcher ) const;
};


#endif //MUSTARD_FILEDIFFLINECONSUMER_H
