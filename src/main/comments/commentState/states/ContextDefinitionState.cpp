#include "ContextDefinitionState.h"
#include "../RegexMatcher.h"
#include "FileDiffState.h"
#include "../lineConsumers/ContextDefinitionConsumer.h"

ContextDefinitionState::ContextDefinitionState ( CommentStateListener *commentStateListener,
        LineClassifier *lineClassifier ) :
    CommentState ( commentStateListener,
                   consumer,
                   lineClassifier ),
    consumer ( commentStateListener ) {}

shared_ptr<CommentState> ContextDefinitionState::traverse ( LineClassifier::LineType lineType )
{
    return make_shared<FileDiffState> ( listener, lineClassifier )->traverse ( lineType );
}


