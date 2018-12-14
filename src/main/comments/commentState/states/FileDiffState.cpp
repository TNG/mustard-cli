#include "FileDiffState.h"
#include "ContextDefinitionState.h"
#include "FileDefinitionState.h"
#include "../RegexMatcher.h"
#include "MultiLineCommentState.h"
#include "../lineConsumers/FileDiffLineConsumer.h"

FileDiffState::FileDiffState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier ) : CommentState (
        commentStateListener,
        new FileDiffLineConsumer ( commentStateListener ), lineClassifier ) {}

CommentState *FileDiffState::traverse ( LineClassifier::LineType lineType )
{
    switch ( lineType ) {
    case LineClassifier::DELLINE:
        return this;
    case LineClassifier::CONTEXTDEFINITION:
        return new ContextDefinitionState ( listener, lineClassifier );
    case LineClassifier::FILEDEFINITION:
        return new FileDefinitionState ( listener, lineClassifier );
    case LineClassifier::MULTILINECOMMENT_START:
        return ( new MultiLineCommentState ( listener, lineClassifier ) )->traverse ( lineType );
    default:
        return this;
    }
}
