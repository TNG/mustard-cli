#include "FileDiffLineConsumer.h"
#include "../RegexMatcher.h"

FileDiffLineConsumer::FileDiffLineConsumer ( CommentStateListener *listener ) :
    listener ( listener )
{

}

void FileDiffLineConsumer::consume ( const string &line )
{
    static RegexMatcher commentMatcher ( R"(^\+.*//~\s*([^~]*)$)" );
    static RegexMatcher cStyleCommentMatcher ( R"(^\+.*/\*~\s*([^~]*)\*/$)" );

    if ( cStyleCommentMatcher.isMatching ( line ) ) {
        addCommentIfMatching ( line, cStyleCommentMatcher );
        return;
    }
    consumeLineNumberIncrease ( line );
    addCommentIfMatching ( line, commentMatcher );
}

void FileDiffLineConsumer::consumeLineNumberIncrease ( const string &line )
{
    static regex increasingLine ( "^[ +].*" );
    if ( regex_match ( line, increasingLine ) ) {
        listener->newLine();
    }
}

void FileDiffLineConsumer::addCommentIfMatching ( const string &line, const RegexMatcher &commentMatcher ) const
{
    const string matchedComment = commentMatcher.getSingleCaptureIn ( line );
    if ( !matchedComment.empty() ) {
        listener->newComment ( "", matchedComment );
    }
}
