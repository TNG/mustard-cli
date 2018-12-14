#ifndef MUSTARD_LINECONSUMER_H
#define MUSTARD_LINECONSUMER_H

#include "CommentStateListener.h"

class LineConsumer
{
public:
    void scopeChange();
    virtual void consume ( const string &line ) {};
    virtual ~LineConsumer() = default;
protected:
    virtual void finishScope() {};
private:
    bool hasBeenFinished = false;
};

#endif //MUSTARD_LINECONSUMER_H