#ifndef MUSTARD_FILEDEFINITIONSTATE_H
#define MUSTARD_FILEDEFINITIONSTATE_H

#include "../CommentState.h"

class FileDefinitionState : public CommentState
{
public:
    FileDefinitionState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier );

    void consume ( const string &line ) override;

    CommentState *traverse ( LineClassifier::LineType lineType ) override;

private:
    bool hasFoundAddFileDefinition = false;
};


#endif //MUSTARD_FILEDEFINITIONSTATE_H
