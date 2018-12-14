#include "FileDefinitionState.h"
#include "../RegexMatcher.h"
#include "ContextDefinitionState.h"
#include "../lineConsumers/FileDefinitionConsumer.h"

FileDefinitionState::FileDefinitionState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier )
    : listener ( commentStateListener ),
      CommentState ( commentStateListener, new FileDefinitionConsumer ( commentStateListener ), lineClassifier ) {}

CommentState *FileDefinitionState::traverse ( LineClassifier::LineType lineType )
{
    if ( lineType == LineClassifier::LineType::CONTEXTDEFINITION ) {
        return new ContextDefinitionState ( listener, lineClassifier );
    }
    return this;
}
