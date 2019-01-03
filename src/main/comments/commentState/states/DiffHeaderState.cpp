#include "DiffHeaderState.h"
#include "FileDefinitionState.h"

DiffHeaderState::DiffHeaderState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier )
    : listener ( commentStateListener ), CommentState ( commentStateListener, new LineConsumer, lineClassifier ) {}

shared_ptr<CommentState> DiffHeaderState::traverse ( LineClassifier::LineType lineType )
{
    if ( lineType == LineClassifier::LineType::FILEDEFINITION ) {
        return make_shared<FileDefinitionState> ( this->listener, this->lineClassifier );
    }
    return shared_from_this();
}

