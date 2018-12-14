#include "ContextDefinitionState.h"
#include "../RegexMatcher.h"
#include "FileDiffState.h"
#include "../lineConsumers/ContextDefinitionConsumer.h"

ContextDefinitionState::ContextDefinitionState ( CommentStateListener *commentStateListener,
        LineClassifier *lineClassifier ) :
    CommentState ( commentStateListener,
                   new ContextDefinitionConsumer (
                       commentStateListener ),
                   lineClassifier ) {}

CommentState *ContextDefinitionState::traverse ( LineClassifier::LineType lineType )
{
    return ( new FileDiffState ( listener, lineClassifier ) )->traverse ( lineType );
}


