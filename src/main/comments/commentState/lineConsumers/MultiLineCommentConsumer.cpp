#include "MultiLineCommentConsumer.h"
#include "../RegexMatcher.h"

MultiLineCommentConsumer::MultiLineCommentConsumer ( CommentStateListener *listener ) : listener ( listener ) {}

void MultiLineCommentConsumer::consume ( const string &line )
{
    static RegexMatcher phantomLine ( R"(^\+.*)" );
    if ( phantomLine.isMatching ( line ) ) {
        listener->newPhantomLine();
    }
    static regex foreignCommentRegex ( R"(^\+\s*/?\*\s*~.*~.*)" );
    if ( regex_match ( line, foreignCommentRegex ) ) {
        extractId ( line );
        foreignComment = true;
        return;
    }
    if ( !comment.empty() ) {
        comment += "\n";
    }
    static RegexMatcher commentOnly ( R"(^\+\s*(?:/\*~|\*)?\s*((?:[^\*]*|\*[^/])*)(?:\*/)?$)" );
    const auto commentString = commentOnly.getSingleCaptureIn ( line );
    comment += commentString.value_or ( "" );
}

void MultiLineCommentConsumer::finishScope()
{
    if ( !foreignComment && !comment.empty() ) {
        listener->newComment ( "", comment );
    }
    comment = "";
}

void MultiLineCommentConsumer::extractId ( const string &line )
{
    static RegexMatcher commentId ( R"(^\+\s*/?\*\s*~[^@]*@(\d*)~.*)" );
    const auto idString = commentId.getSingleCaptureIn ( line );
    if ( idString.has_value() ) {
        id = atol ( idString.value().c_str() );
    }
}
