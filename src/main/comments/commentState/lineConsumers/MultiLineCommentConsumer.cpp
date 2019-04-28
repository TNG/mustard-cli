#include <Depend.h>
#include "MultiLineCommentConsumer.h"
#include "../RegexMatcher.h"

MultiLineCommentConsumer::MultiLineCommentConsumer ( CommentStateListener *listener, MultiLineCommentConsumer *inReplyTo, TagExtractor *tagExtractor ) :
    listener ( listener ),
    inReplyTo ( inReplyTo ),
    tagExtractor ( DependentOn<TagExtractor> ( tagExtractor ) ) {}

void MultiLineCommentConsumer::consume ( const string &line )
{
    static RegexMatcher phantomLine ( R"(^\+.*)" );
    if ( phantomLine.isMatching ( line ) ) {
        listener->newPhantomLine();
    }

    vector<Tag> tagsOfLine = tagExtractor->extractTagsIn ( line );
    for ( const auto &tag : tagsOfLine ) {
        if ( tag.name == "id" && tag.value.has_value() ) {
            id = atol ( tag.value.value().c_str() );
        }
        if ( tag.name == "inReplyTo" && tag.value.has_value() ) {
            inReplyToId = atol ( tag.value.value().c_str() );
        }
        if ( tag.name == "author" && tag.value.has_value() ) {
            author = tag.value;
        }
    }
    string lineWithoutTags = tagExtractor->removeTagsFrom ( line );
    if ( !comment.empty() ) {
        comment += "\n";
    }
    static RegexMatcher commentOnly ( R"(^\+[\s*~/]*((?:[^\*]*|\*[^/])*)(?:\*/)?$)" );
    const auto commentString = commentOnly.getSingleCaptureIn ( lineWithoutTags );
    comment += commentString.value_or ( "" );
}

void MultiLineCommentConsumer::finishScope()
{
    if ( !comment.empty() ) {
        optional<unsigned long >replyToId = inReplyToId;
        if ( !inReplyToId.has_value() && inReplyTo != nullptr ) {
            replyToId = inReplyTo->getId();
        }
        listener->newComment ( author.value_or ( "" ), comment, id, replyToId );
    }
    comment = "";
}
