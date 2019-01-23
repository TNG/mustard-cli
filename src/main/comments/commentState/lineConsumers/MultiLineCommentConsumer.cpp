#include "MultiLineCommentConsumer.h"
#include "../RegexMatcher.h"

MultiLineCommentConsumer::MultiLineCommentConsumer ( CommentStateListener *listener ) : listener ( listener ) {}

void MultiLineCommentConsumer::consume ( const string &line )
{
    static RegexMatcher phantomLine ( R"(^\+.*)" );
    if ( phantomLine.isMatching ( line ) ) {
        listener->newPhantomLine();
    }

    static regex foreignCommentRegex ( R"(^\+\s*/\*~.*~.*)" );
    if ( foreignComment ||  regex_match ( line, foreignCommentRegex ) ) {
        foreignComment = true;
        return;
    }
    if ( !comment.empty() ) {
        comment += "\n";
    }
    static RegexMatcher commentOnly ( R"(^\+\s*(?:/\*~|\*)?\s*((?:[^\*]*|\*[^/])*)(?:\*/)?$)" );
    comment += commentOnly.getSingleCaptureIn ( line );
}

void MultiLineCommentConsumer::finishScope()
{
    if ( !foreignComment && !comment.empty() ) {
        listener->newComment ( "", comment );
    }
}
