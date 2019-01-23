#ifndef MUSTARD_MULTILINECOMMENTCONSUMER_H
#define MUSTARD_MULTILINECOMMENTCONSUMER_H


#include <optional>
#include "../CommentStateListener.h"
#include "../LineConsumer.h"

using namespace std;

class MultiLineCommentConsumer : public LineConsumer
{
public:
    MultiLineCommentConsumer ( CommentStateListener *listener, MultiLineCommentConsumer *inReplyTo = nullptr );
    void consume ( const string &line ) override;

    optional<long> getId() {
        return id;
    };
protected:
    void finishScope() override;

private:
    CommentStateListener *listener;
    string comment;
    optional<unsigned long> id;
    MultiLineCommentConsumer *inReplyTo;
    bool foreignComment = false;

    void extractId ( const string &line );
};


#endif //MUSTARD_MULTILINECOMMENTCONSUMER_H
