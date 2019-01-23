#include "MultiLineCommentConsumer.h"
#include "../RegexMatcher.h"

MultiLineCommentConsumer::MultiLineCommentConsumer ( CommentStateListener *listener, MultiLineCommentConsumer *inReplyTo ) :
    listener ( listener ),
    inReplyTo ( inReplyTo ) {}

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
    }
    if ( !comment.empty() ) {
        comment += "\n";
    }
    static RegexMatcher commentOnly ( R"(^\+\s*(?:/\*~|\*)?\s*(?:@\w+\s*)?((?:[^\*]*|\*[^/])*)(?:\*/)?$)" );
    const auto commentString = commentOnly.getSingleCaptureIn ( line );
    comment += commentString.value_or ( "" );
}

void MultiLineCommentConsumer::finishScope()
{
    if ( !foreignComment && !comment.empty() ) {
        optional<unsigned long >replyToId;
        if ( inReplyTo != nullptr ) {
            replyToId = inReplyTo->getId();
        }
        listener->newComment ( "", comment, replyToId );
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
