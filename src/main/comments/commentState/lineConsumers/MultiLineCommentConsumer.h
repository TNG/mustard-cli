#ifndef MUSTARD_MULTILINECOMMENTCONSUMER_H
#define MUSTARD_MULTILINECOMMENTCONSUMER_H


#include "../CommentStateListener.h"
#include "../LineConsumer.h"

class MultiLineCommentConsumer : public LineConsumer
{
public:
    MultiLineCommentConsumer ( CommentStateListener *listener );
private:
    CommentStateListener *listener;
public:
    void consume ( const string &line ) override;

protected:
    void finishScope() override;

private:
    string comment;
    bool foreignComment = false;

};


#endif //MUSTARD_MULTILINECOMMENTCONSUMER_H
