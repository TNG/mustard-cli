#include "FileDefinitionState.h"
#include "../RegexMatcher.h"
#include "ContextDefinitionState.h"
#include "../lineConsumers/FileDefinitionConsumer.h"

FileDefinitionState::FileDefinitionState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier )
    : listener ( commentStateListener ),
      CommentState ( commentStateListener, new FileDefinitionConsumer ( commentStateListener ), lineClassifier ) {}

shared_ptr<CommentState> FileDefinitionState::traverse ( LineClassifier::LineType lineType )
{
    if ( lineType == LineClassifier::LineType::CONTEXTDEFINITION ) {
        return make_shared<ContextDefinitionState> ( listener, lineClassifier );
    }
    return shared_from_this();
}
