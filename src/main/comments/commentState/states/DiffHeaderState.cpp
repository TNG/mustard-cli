#include "DiffHeaderState.h"
#include "FileDefinitionState.h"

DiffHeaderState::DiffHeaderState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier )
    : listener ( commentStateListener ), CommentState ( commentStateListener, new LineConsumer, lineClassifier ) {}

CommentState *DiffHeaderState::traverse ( LineClassifier::LineType lineType )
{
    if ( lineType == LineClassifier::LineType::FILEDEFINITION ) {
        return new FileDefinitionState ( this->listener, this->lineClassifier );
    }
    return this;
}

