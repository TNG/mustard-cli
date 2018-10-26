#include <functional>
#include "../../main/comments/CommentConsumer.h"
#include "CommentMatcher.h"

CommentMatcher::CommentMatcher ( std::function<bool ( const string &file, const LineComment & ) > predicate ) :
    predicate ( std::move ( predicate ) ) {}

void CommentMatcher::consume ( const string &file, const LineComment &lineComment )
{
    matched |= predicate ( file, lineComment );
}

bool CommentMatcher::isMatching()
{
    return matched ;
}
