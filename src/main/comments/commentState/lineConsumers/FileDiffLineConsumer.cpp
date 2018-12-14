#include "FileDiffLineConsumer.h"
#include "../RegexMatcher.h"

FileDiffLineConsumer::FileDiffLineConsumer ( CommentStateListener *listener ) :
    listener ( listener )
{

}

void FileDiffLineConsumer::consume ( const string &line )
{
    consumeLineNumberIncrease ( line );
    consumeInlineComment ( line );
}

void FileDiffLineConsumer::consumeLineNumberIncrease ( const string &line )
{
    static regex increasingLine ( "^[ +].*" );
    if ( regex_match ( line, increasingLine ) ) {
        listener->newLine();
    }
}

void FileDiffLineConsumer::consumeInlineComment ( const string &line ) const
{
    static RegexMatcher commentMatcher ( R"(^\+.*//([^~]*)$)" );
    const string matchedComment = commentMatcher.getSingleCaptureIn ( line );
    if ( !matchedComment.empty() ) {
        listener->newComment ( "", matchedComment );
    }
}
