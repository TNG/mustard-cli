#ifndef MUSTARD_COMMENTMATCHER_H
#define MUSTARD_COMMENTMATCHER_H

#include <functional>
#include "../../main/comments/CommentConsumer.h"

class CommentMatcher : public CommentConsumer
{
public:
    CommentMatcher ( std::function<bool ( const string &file, const LineComment & ) > predicate );;

    void consume ( const string &file, const LineComment &lineComment ) override;

    bool isMatching();;

private:
    std::function<bool ( const string &, const LineComment & ) > predicate;
    bool matched = false;
};

#endif //MUSTARD_COMMENTMATCHER_H
