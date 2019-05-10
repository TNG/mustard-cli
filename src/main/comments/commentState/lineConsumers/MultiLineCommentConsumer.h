#ifndef MUSTARD_MULTILINECOMMENTCONSUMER_H
#define MUSTARD_MULTILINECOMMENTCONSUMER_H

#include <optional>
#include "../CommentStateListener.h"
#include "../LineConsumer.h"
#include "../../../comments/TagExtractor.h"

using namespace std;

class MultiLineCommentConsumer : public LineConsumer
{
public:
    MultiLineCommentConsumer ( CommentStateListener *listener, MultiLineCommentConsumer *inReplyTo = nullptr, TagExtractor *tagExtractor = nullptr );
    void consume ( const string &line ) override;

    optional<long> getId() {
        return id;
    };
protected:
    void finishScope() override;

private:

    CommentStateListener *listener;
    string comment;
    optional<unsigned long> id = {};
    optional<unsigned long> inReplyToId = {};
    optional<string> author = {};

    MultiLineCommentConsumer *inReplyTo;
    TagExtractor *tagExtractor;
};


#endif //MUSTARD_MULTILINECOMMENTCONSUMER_H
