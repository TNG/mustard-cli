#include "FileDiffState.h"
#include "ContextDefinitionState.h"
#include "FileDefinitionState.h"
#include "../RegexMatcher.h"
#include "MultiLineCommentState.h"
#include "../lineConsumers/FileDiffLineConsumer.h"

FileDiffState::FileDiffState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier ) :
    CommentState (
        commentStateListener,
        consumer, lineClassifier ),
    consumer (
        commentStateListener ) {}

shared_ptr<CommentState> FileDiffState::traverse ( LineClassifier::LineType lineType )
{
    switch ( lineType ) {
    case LineClassifier::DELLINE:
        return shared_from_this();
    case LineClassifier::CONTEXTDEFINITION:
        return make_shared<ContextDefinitionState> ( listener, lineClassifier );
    case LineClassifier::FILEDEFINITION:
        return make_shared<FileDefinitionState> ( listener, lineClassifier );
    case LineClassifier::MULTILINECOMMENT_START:
        return make_shared<MultiLineCommentState> ( listener, lineClassifier )->traverse ( lineType );
    default:
        return shared_from_this();
    }
}
