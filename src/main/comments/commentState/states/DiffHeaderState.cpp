#include "DiffHeaderState.h"
#include "FileDefinitionState.h"

DiffHeaderState::DiffHeaderState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier )
    : CommentState ( commentStateListener, lineClassifier ) {}

CommentState *DiffHeaderState::traverse ( LineClassifier::LineType lineType )
{
    if ( lineType == LineClassifier::LineType::FILEDEFINITION ) {
        return new FileDefinitionState ( this->listener, this->lineClassifier );
    }
    return this;
}

