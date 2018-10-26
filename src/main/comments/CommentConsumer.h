#ifndef MUSTARD_COMMENTCONSUMER_H
#define MUSTARD_COMMENTCONSUMER_H

#include <string>
#include "LineComment.h"

using namespace std;

class CommentConsumer
{
public:
    virtual void consume ( const string &file, const LineComment &lineComment ) = 0;
};

#endif //MUSTARD_COMMENTCONSUMER_H
