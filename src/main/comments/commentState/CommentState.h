#ifndef MUSTARD_COMMENTSTATE_H
#define MUSTARD_COMMENTSTATE_H

#include <string>
#include <Depend.h>
#include "CommentStateListener.h"
#include "LineClassifier.h"
#include "LineConsumer.h"

using namespace std;

class CommentState : public enable_shared_from_this<CommentState>
{
public:
    CommentState ( CommentStateListener *listener, LineConsumer &consumer, LineClassifier *lineClassifier = nullptr ) :
        listener ( listener ),
        lineClassifier ( DependentOn<LineClassifier> ( lineClassifier ) ),
        consumer ( &consumer ) {}

    virtual void consume ( const string &line ) {
        consumer->consume ( line );
    }
    void scopeChange() {
        consumer->scopeChange();
    }
    virtual shared_ptr<CommentState> traverse ( LineClassifier::LineType lineType ) {
        return shared_from_this();
    }

    virtual ~CommentState() {};
private:

protected:
    CommentStateListener *listener;
    LineConsumer *consumer;
    LineClassifier *lineClassifier;
};


#endif //MUSTARD_COMMENTSTATE_H
