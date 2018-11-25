#ifndef MUSTARD_COMMENTSTATE_H
#define MUSTARD_COMMENTSTATE_H

#include <string>
#include <Depend.h>
#include "CommentStateListener.h"
#include "LineClassifier.h"

using namespace std;

class CommentState
{
public:
    CommentState ( CommentStateListener *commentStateListener, LineClassifier *lineClassifier = nullptr ) :
        listener ( commentStateListener ),
        lineClassifier ( DependentOn<LineClassifier> ( lineClassifier ) )
    {}

    virtual void consume ( const string &line ) {}
    virtual CommentState *traverse ( LineClassifier::LineType lineType ) {
        return this;
    }

    virtual ~CommentState() = default;

protected:
    CommentStateListener *listener;
    LineClassifier *lineClassifier;

};


#endif //MUSTARD_COMMENTSTATE_H
