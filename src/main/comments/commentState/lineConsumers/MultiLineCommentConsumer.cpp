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
            continue;
        }
        if ( tag.name == "inReplyTo" && tag.value.has_value() ) {
            inReplyToId = atol ( tag.value.value().c_str() );
            continue;
        }
        if ( tag.name == "author" && tag.value.has_value() ) {
            author = tag.value;
            continue;
        }
        if ( tag.name == "todo" && tag.value.has_value() ) {
            todos.emplace_back ( ( Todo ) {
                tag.value.value(), Todo::TODO
            } );
            continue;
        }
        if ( tag.name == "done" && tag.value.has_value() ) {
            todos.emplace_back ( ( Todo ) {
                tag.value.value(), Todo::DONE
            } );
            continue;
        }
    }
    string lineWithoutTags = tagExtractor->removeTagsFrom ( line );
    static RegexMatcher commentOnly ( R"(^\+[\s*~/]*((?:[^\*]*|\*[^/])*)(?:\*/)?$)" );
    const auto commentString = commentOnly.getSingleCaptureIn ( lineWithoutTags );
    if ( commentString.has_value() && !commentString.value().empty() ) {
        if ( !comment.empty() ) {
            comment += "\n";
        }
        comment += commentString.value_or ( "" );
    }
}

void MultiLineCommentConsumer::finishScope()
{
    if ( !comment.empty() ) {
        optional<unsigned long >replyToId = inReplyToId;
        if ( !inReplyToId.has_value() && inReplyTo != nullptr ) {
            replyToId = inReplyTo->getId();
        }
        listener->newComment ( author.value_or ( "" ), comment, id, replyToId );
        listener->withTodos ( todos );
    }
    comment = "";
}
